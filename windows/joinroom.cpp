#include "joinroom.h"
#include "datamanager.h"
#include "ui_joinroom.h"

#include <codec.h>

JoinRoom::JoinRoom(DialogType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JoinRoom)
{
    ui->setupUi(this);
    setFixedSize(400, 200);

    ui->joinRoomBtn->setFixedSize(100, 45);
    ui->joinRoomBtn->setImage(":/images/addRoom-normal.png",
                              ":/images/addRoom-hover.png",
                              ":/images/addRoom-pressed.png",
                              ":/images/addRoom-disable.png");
    ui->joinRoomBtn->setBtnDisable(true);

    const QString titles[] =
    {
        ":/images/info1.png",
        ":/images/info1.png"
    };

    int index = static_cast<int>(type);
    ui->descript->setPixmap(QPixmap(titles[index]));
    ui->stackedWidget->setCurrentIndex(index);

    Communication* comm = DataManager::getInstance()->getCommunication();
    connect(comm, &Communication::roomExist, this, [=](bool flag)
    {
        if(flag)
        {
            ui->joinRoomBtn->setBtnDisable(false);
        }
        else
        {
            ui->joinRoomBtn->setBtnDisable(true);
        }
    });


    connect(ui->createRoomBtn, &QPushButton::clicked, this, &JoinRoom::joinRoom);
    connect(ui->searchRoomBtn, &QPushButton::clicked, this, &JoinRoom::searchRoom);
    connect(ui->joinRoomBtn, &QPushButton::clicked, this, &JoinRoom::joinRoom);


}

JoinRoom::~JoinRoom()
{
    delete ui;
}

void JoinRoom::encodeMessage(RequestCode code)
{
    Message msg;
    msg.reqCode = code;
    msg.userName = DataManager::getInstance()->getUserName();
    msg.roomName = ui->roomName->text().toUtf8();
    DataManager::getInstance()->getCommunication()->sendMessage(&msg);
}

void JoinRoom::searchRoom()
{
    encodeMessage(RequestCode::SearchRoom);
}

void JoinRoom::joinRoom()
{
    encodeMessage(RequestCode::ManualRoom);
    DataManager::getInstance()->setRoomMode(DataManager::Manual);
    accept();
}
