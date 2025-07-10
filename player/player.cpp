#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{
}

Player::Player(QString name, QObject *parent) : Player{parent}
{
    m_name = name;
}

void Player::setName(QString name)
{
    m_name = name;
}

QString Player::getName()
{
    return m_name;
}

void Player::setRole(Role role)
{
    m_role = role;
}

Player::Role Player::getRole()
{
    return m_role;
}

void Player::setSex(Sex sex)
{
    m_sex = sex;
}

Player::Sex Player::getSex()
{
    return m_sex;
}

void Player::setDirection(Direction dir)
{
    m_dir = dir;
}

Player::Direction Player::getDirection()
{
    return m_dir;
}

void Player::setType(Type type)
{
    m_type = type;
}

Player::Type Player::getType()
{
    return m_type;
}

void Player::setScore(int score)
{
    m_score = score;
}

int Player::getScore()
{
    return m_score;
}

void Player::setWin(bool flag)
{
    m_isWin = flag;
}

bool Player::getWin()
{
    return m_isWin;
}

void Player::setPrevPlayer(Player *player)
{
    m_prev = player;
}

void Player::setNextPlayer(Player *player)
{
    m_next = player;
}

Player *Player::getPrevPlayer()
{
    return m_prev;
}

Player *Player::getNextPlayer()
{
    return m_next;
}

void Player::grabLordBet(int point)
{
    emit notifyGrabLordBet(this, point);
}

void Player::storeDispatchCard(const Card &card)
{
    m_cards.add(card);
    Cards cs;
    cs.add(card);
    emit notifyPickCards(this, cs);
}

void Player::storeDispatchCard(const Cards &cards)
{
    m_cards.add(cards);
    emit notifyPickCards(this, cards);
}

Cards Player::getCards()
{
    return m_cards;
}

void Player::clearCards()
{
    m_cards.clear();
    sethasPlayLeftCard2(false);
    sethasPlayLeftCard1(false);
}

void Player::playHand(Cards &cards)
{
    m_cards.remove(cards);
    //Cards tmp = cards;
    emit notifyPlayHand(this, cards);
}

void Player::sethasPlayLeftCard2(bool flag)
{
    m_hasPlayLeftCard2 = flag;
}

void Player::sethasPlayLeftCard1(bool flag)
{
    m_hasPlayLeftCard1 = flag;
}

bool Player::gethasPlayLeftCard2()
{
    return m_hasPlayLeftCard2;
}

bool Player::gethasPlayLeftCard1()
{
    return m_hasPlayLeftCard1;
}

void Player::setPendingInfo(Player *player, const Cards &cards)
{
    m_pendPlayer = player;
    m_pendCards = cards;
}

Player *Player::getPendPlayer()
{
    return m_pendPlayer;
}

Cards Player::getPendCards()
{
    return m_pendCards;
}

void Player::storePendingInfo(Player *player, const Cards &cards)
{
    m_pendPlayer = player;
    m_pendCards = cards;
}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}

void Player::thinkCallLord()
{

}

void Player::thinkPlayHand()
{

}
