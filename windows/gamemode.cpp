#include "gamemode.h"
#include "datamanager.h"
#include "ui_gamemode.h"
#include "gamepanel.h"

#include <JoinRoom.h>
#include <codec.h>

GameMode::GameMode(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GameMode)
{
    ui->setupUi(this);
    setFixedSize(800, 500);
    ui->stackedWidget->setCurrentIndex(0);
    ui->information->setVisible(false);

    //取出通信类的实例对象
    Communication* comm = DataManager::getInstance()->getCommunication();
    connect(comm, &Communication::playerCount, this, [=](int count)
    {
        showInfo(count);
        ui->information->setVisible(true);
    });

    connect(comm, &Communication::somebodyLeave, this, &GameMode::showInfo);

    //开始游戏
    connect(comm, &Communication::startGame, this, [=](QByteArray msg)
    {
        if(this->isVisible())
        {
            this->hide();
            GamePanel* pannel = new GamePanel;
            connect(pannel, &GamePanel::panelClose, this, &GameMode::show);
            pannel->show();
            //初始化数据
            pannel->initGamePanel(msg);
        }


        //断开信号槽连接
        // disconnect(comm, &Communication::startGame, this, nullptr);
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](int index)
    {
        if(index == 0)
        {
            ui->information->setVisible(false);
        }
    });

    //单机模式
    connect(ui->singleModeBtn, &QPushButton::clicked, this, [=]()
    {
        GamePanel* panel = new GamePanel;

        connect(panel, &GamePanel::panelClose, this, &GameMode::show);

        panel->show();
        hide();

        DataManager::getInstance()->setGameMode(DataManager::Single);
    });

    //网络模式
    connect(ui->netModeBtn, &QPushButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentIndex(1);
        DataManager::getInstance()->setGameMode(DataManager::NetWork);
    });

    //自动加入某个房间
    connect(ui->autoBtn, &QPushButton::clicked, this, [=]()
    {
        Message msg;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.reqCode = RequestCode::AutoRoom;
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        DataManager::getInstance()->setRoomMode(DataManager::Auto);
    });

    //手动创建房间
    connect(ui->manualBtn, &QPushButton::clicked, this, [=]()
    {
        JoinRoom room(DialogType::Manual);
        room.setWindowTitle("创建房间");
        room.exec();
    });

    //搜索房间
    connect(ui->searchBtn, &QPushButton::clicked, this, [=]()
    {
        JoinRoom room(DialogType::Search);
        room.setWindowTitle("搜索房间");
        room.exec();
    });



}

GameMode::~GameMode()
{
    delete ui;
}

void GameMode::showInfo(int count)
{
    QString tip = QString("当前<%1>房间玩家人数为 %2 人，正在等待其他玩家进入，请稍后...").arg(DataManager::getInstance()->getRoomName().data()).arg(count);
    ui->information->setText(tip);
}

void GameMode::closeEvent(QCloseEvent *event)
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->stackedWidget->setCurrentIndex(0);
        event->ignore();
    }
    else
    {
        event->accept();
        Message msg;
        msg.reqCode = RequestCode::GoodBye;
        msg.userName = DataManager::getInstance()->getUserName();
        msg.roomName = DataManager::getInstance()->getRoomName();
        DataManager::getInstance()->getCommunication()->sendMessage(&msg);
        DataManager::getInstance()->getCommunication()->setStop();

        // 等待线程池中的任务完成，但设置超时
        QThreadPool::globalInstance()->clear(); // 清除未开始的任务
        bool finished = QThreadPool::globalInstance()->waitForDone(2000); // 等待2秒

        if(!finished)
        {
            qDebug() << "Force terminating thread pool tasks" << Qt::endl;
        }
        
        // 强制退出应用程序
        QApplication::quit();
    }
}
