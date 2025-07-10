#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::initButton()
{
    //开始游戏
    ui->start->setImage(":/images/start-1.png", ":/images/start-3.png", ":/images/start-2.png");

    //出牌
    ui->playCard->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    ui->playCard1->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");

    //不要
    ui->pass->setImage(":/images/pass_btn-1.png", ":/images/pass_btn-3.png", ":/images/pass_btn-2.png");

    //不抢
    ui->giveup->setImage(":/images/buqiang-1.png", ":/images/buqiang-3.png", ":/images/buqiang-2.png");

    //1，2，3分
    ui->onescore->setImage(":/images/1fen-1.png", ":/images/1fen-3.png", ":/images/1fen-2.png");
    ui->twoscore->setImage(":/images/2fen-1.png", ":/images/2fen-3.png", ":/images/2fen-2.png");
    ui->threescore->setImage(":/images/3fen-1.png", ":/images/3fen-3.png", ":/images/3fen-2.png");

    //设置按钮的大小
    QVector<MyButton*> btns;
    btns << ui->start << ui->playCard << ui->playCard1 << ui->pass << ui->giveup << ui->onescore << ui->twoscore << ui->threescore;

    for(int i = 0; i < btns.size(); ++i)
    {
        btns[i]->setFixedSize(90, 45);
    }

    connect(ui->start, &MyButton::clicked, this, &ButtonGroup::startGame);
    connect(ui->playCard, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(ui->playCard1, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(ui->pass, &MyButton::clicked, this, &ButtonGroup::pass);
    connect(ui->giveup, &MyButton::clicked, this, [=]()
    {
        emit betPoint(0);
    });

    connect(ui->onescore, &MyButton::clicked, this, [=]()
    {
        emit betPoint(1);
    });

    connect(ui->twoscore, &MyButton::clicked, this, [=]()
    {
        emit betPoint(2);
    });

    connect(ui->threescore, &MyButton::clicked, this, [=]()
    {
        emit betPoint(3);
    });

}

void ButtonGroup::selectPanel(Panel type, int bet)
{
    ui->stackedWidget->setCurrentIndex(type);
    if(type != CallLord)
    {
        return;
    }
    if(bet == 0)
    {
        ui->onescore->setVisible(true);
        ui->twoscore->setVisible(true);
        ui->threescore->setVisible(true);
    }
    else if(bet == 1)
    {
        ui->onescore->setVisible(false);
        ui->twoscore->setVisible(true);
        ui->threescore->setVisible(true);
    }
    else if(bet == 2)
    {
        ui->onescore->setVisible(false);
        ui->twoscore->setVisible(false);
        ui->threescore->setVisible(true);
    }
}


