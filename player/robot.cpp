#include "robot.h"
#include "datamanager.h"

Robot::Robot(QObject *parent)
    : Player{parent}
{
    m_type = Type::Robot;
}

#include <QDebug>
#include <taskqueue.h>
void Robot::prepareCallLord()
{
    RobotGrapLord* task = new RobotGrapLord(this);
    QThreadPool::globalInstance()->start(task);

    // connect(subThread, &RobotGrapLord::finished, this, [=]()
    // {
    //     qDebug() << "RobotGrapLord 子线程对象析构..." << "Robot name: " << this->getName();
    //     subThread->deleteLater();
    // });

    // subThread->start();
}

void Robot::preparePlayHand()
{
    RobotPlayHand* task = new RobotPlayHand(this);
    QThreadPool::globalInstance()->start(task);

    // connect(subThread, &RobotPlayHand::finished, this, [=]()
    // {
    //     qDebug() << "RobotPlayHand 子线程对象析构..." << "Robot name: " << this->getName();
    //     subThread->deleteLater();
    // });

    // subThread->start();
}

void Robot::thinkPlayHand()
{
    if(DataManager::getInstance()->isNetworkMode())
    {
        Task t = TaskQueue::getInstance()->take();
        playHand(t.cards);
        return;
    }

    Strategy st(this, m_cards);

    Cards cs = st.makeStrategy();

    playHand(cs);
}



void Robot::thinkCallLord()
{
    /*
     * 基于手中的牌计算权重
     * 大小王：6
     * 顺子/炸弹：5
     * 三张牌一样：4
     * 2的权重：3
     * 对儿牌：1
    */

    if(DataManager::getInstance()->isNetworkMode())
    {
        Task t = TaskQueue::getInstance()->take();
        grabLordBet(t.bet);
        return;
    }

    int weight = 0;
    Strategy st(this, m_cards);

    //大小王
    weight += st.getRangeCards(Card::Card_SJ, Card::Card_BJ).cardCount() * 6;


    //顺子
    QVector<Cards> optSeq = st.pickOptimalSeqSingles();
    weight += optSeq.size() * 5;


    //炸弹
    QVector<Cards> bombs = st.findCardsByCount(4);
    weight += bombs.size() * 5;

    //2的数量
    weight += m_cards.pointCount(Card::Card_2) * 3;

    Cards tmp = m_cards;
    tmp.remove(optSeq);
    tmp.remove(bombs);
    Cards cards2 = st.getRangeCards(Card::Card_2, Card::Card_2);
    tmp.remove(cards2);


    //三张牌一样
    QVector<Cards> triples = Strategy(this, tmp).findCardsByCount(3);
    weight += triples.size() * 4;

    tmp.remove(triples);

    //对儿牌
    QVector<Cards> pairs = Strategy(this, tmp).findCardsByCount(2);
    weight += pairs.size();


    if(weight >= 22)
    {
        grabLordBet(3);
    }
    else if(weight >= 18)
    {
        grabLordBet(2);
    }
    else if(weight >= 10)
    {
        grabLordBet(1);
    }
    else
    {
        grabLordBet(0);
    }

}

