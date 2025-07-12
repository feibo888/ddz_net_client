#include "gamecontrol.h"

#include <QDebug>
#include "datamanager.h"
#include "codec.h"

GameControl::GameControl(QObject *parent)
    : QObject{parent}
{}

void GameControl::playerInit()
{
    m_robotLeft = new Robot("机器人A", this);
    m_robotRight = new Robot("机器人B", this);
    m_user = new UserPlayer("自己", this);

    //头像显示
    m_robotLeft->setDirection(Player::Left);
    m_robotRight->setDirection(Player::Right);
    m_user->setDirection(Player::Right);

    //性别
    Player::Sex sex;
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);


    //出牌顺序
    //user
    m_user->setPrevPlayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);

    //left robot
    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);

    //right robot
    m_robotRight->setPrevPlayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);

    //指定当前玩家
    m_currPlayer = m_user;

    m_pendPlayer = nullptr;

    //处理玩家发射出的信号
    connect(m_user, &Player::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotLeft, &Player::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotRight, &Player::notifyGrabLordBet, this, &GameControl::onGrabBet);

    //传递出牌玩家对象和玩家打出的牌
    connect(this, &GameControl::pendingInfo, m_robotLeft, &Player::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_robotRight, &Player::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_user, &Player::storePendingInfo);

    //处理玩家出牌
    connect(m_robotLeft, &Player::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_robotRight, &Player::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_user, &Player::notifyPlayHand, this, &GameControl::onPlayHand);

}

Robot *GameControl::getLeftRobot()
{
    return m_robotLeft;
}

Robot *GameControl::getRightRobot()
{
    return m_robotRight;
}

UserPlayer *GameControl::getUserPlayer()
{
    return m_user;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_currPlayer = player;
}

Player *GameControl::getCurrentPlayer()
{
    return m_currPlayer;
}

Player *GameControl::getPendPlayer()
{
    return m_pendPlayer;
}

Cards GameControl::getPendCards()
{
    return m_pendCards;
}

void GameControl::initAllCards()
{
    //判断是不是网络模式
    if(DataManager::getInstance()->isNetworkMode())
    {
        m_allCards = DataManager::getInstance()->getCards();
        m_allCards.add(DataManager::getInstance()->getLastCards());
        return;
    }

    m_allCards.clear();
    for(int p = Card::Card_Begin + 1; p < Card::Card_SJ; ++p)
    {
        for(int s = Card::Suit_Begin + 1; s < Card::Suit_End; ++s)
        {
            Card c((Card::CardPoint)p, (Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJ, Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ, Card::Suit_Begin));
}

Card GameControl::takeOneCard()
{
    return m_allCards.takeRandCard();
}

Cards GameControl::getSurplusCards()
{
    return m_allCards;
}

void GameControl::resetCardData()
{
    //洗牌
    initAllCards();
    //清空玩家所有牌
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();
    //初始化出牌玩家和牌
    m_pendPlayer = nullptr;
    m_pendCards.clear();

}

void GameControl::startLordCard()
{
    m_currPlayer->prepareCallLord();
    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
}

void GameControl::becomeLord(Player* player, int bet)
{
    m_currBet = bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);

    m_currPlayer = player;
    player->storeDispatchCard(m_allCards);

    QTimer::singleShot(1000, this, [=]()
    {
        emit gameStatusChanged(PlayingHand);
        emit playerStatusChanged(player, ThinkingForPlayHand);
        m_currPlayer->preparePlayHand();
    });

}

void GameControl::clearPlayerScore()
{
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

int GameControl::getPlayerMaxBet()
{
    return m_record.bet;
}

void GameControl::onGrabBet(Player *player, int bet)
{
    //qDebug() << bet << Qt::endl;
    //1. 通知主界面玩家叫地主了
    if(bet == 0 || m_record.bet >= bet)
    {
        emit notifyGrabLordBet(player, 0, false);
    }
    else if(bet > 0 && m_record.bet == 0)
    {
        //第一个抢地主的玩家
        emit notifyGrabLordBet(player, bet, true);
    }
    else
    {
        //第二三个抢地主的玩家
        emit notifyGrabLordBet(player, bet, false);
    }

    qDebug() << "curent player name: " << player->getName() << ", 下注分数: "
             << bet<< ", m_betRecord.times: " << m_record.times;


    //2. 判断玩家下注是不是3分，如果是就结束
    if(bet == 3)
    {
        becomeLord(player, bet);
        m_record.reset();
        return;
    }

    //3. 下注不够3分，对玩家的分数进行比较，最高的是地主
    if(m_record.bet < bet)
    {
        m_record.bet = bet;
        m_record.player = player;
    }
    m_record.times++;

    if(m_record.times == 3)
    {
        if(m_record.bet == 0)
        {
            // 所有玩家都不抢地主，需要重新发牌
            if(DataManager::getInstance()->isNetworkMode())
            {
                // 网络模式：向服务器请求重新发牌
                Message msg;
                msg.userName = DataManager::getInstance()->getUserName();
                msg.roomName = DataManager::getInstance()->getRoomName();
                msg.reqCode = RequestCode::ReDealCards;
                DataManager::getInstance()->getCommunication()->sendMessage(&msg);
            }
            else
            {
                // 单机模式：直接重新发牌
                emit gameStatusChanged(GameStatus::DispatchCard);
            }
        }
        else
        {
            becomeLord(m_record.player, m_record.bet);
        }
        m_record.reset();
        return;
    }

    //4. 切换玩家，通知下一个玩家继续抢地主
    m_currPlayer = player->getNextPlayer();

    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
    m_currPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player *player, Cards &cards)
{
    //将玩家出牌的信号发送给主界面
    emit notifyPlayHand(player, cards);

    //如果不是空牌，给其它玩家发送信号，保存出牌玩家对象和打出牌
    if(!cards.isEmpty())
    {
        m_pendPlayer = player;
        m_pendCards = cards;
        emit pendingInfo(player, cards);
    }

    //如果有炸弹，低分翻倍
    PlayHand::HandType type = PlayHand(cards).getHandType();
    if(type == PlayHand::HandType::Hand_Bomb || type == PlayHand::HandType::Hand_Bomb_Jokers)
    {
        m_currBet *= 2;
    }

    //如果玩家的牌出完了，计算本局游戏的总分
    if(player->getCards().isEmpty())
    {
        Player* pre = player->getPrevPlayer();
        Player* next = player->getNextPlayer();
        if(player->getRole() == Player::Lord)
        {
            player->setScore(player->getScore() + 2 * m_currBet);
            pre->setScore(pre->getScore() - m_currBet);
            next->setScore(next->getScore() - m_currBet);

            player->setWin(true);
            pre->setWin(false);
            next->setWin(false);
        }
        else
        {
            player->setWin(true);
            player->setScore(player->getScore() + m_currBet);
            if(pre->getRole() == Player::Lord)
            {
                pre->setScore(pre->getScore() - 2 * m_currBet);
                pre->setWin(false);
                next->setScore(next->getScore() + m_currBet);
                next->setWin(true);
            }
            else
            {
                pre->setScore(pre->getScore() + m_currBet);
                pre->setWin(true);
                next->setScore(next->getScore() - 2 * m_currBet);
                next->setWin(false);
            }
        }
        emit playerStatusChanged(player, GameControl::Winning);
        return;
    }

    //牌没有出完，下一个玩家继续出牌
    m_currPlayer = player->getNextPlayer();
    m_currPlayer->preparePlayHand();
    emit playerStatusChanged(m_currPlayer, GameControl::ThinkingForPlayHand);
}

void GameControl::setCurrentPlayer(int index)
{
    if(index == 1)
    {
        m_currPlayer = m_user;
    }
    else if(index == 2)
    {
        m_currPlayer = m_robotLeft;
    }
    else if(index == 3)
    {
        m_currPlayer = m_robotRight;
    }
}
