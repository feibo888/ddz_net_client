#include "card.h"

#include <QDataStream>

Card::Card() {}

Card::Card(CardPoint point, CardSuit suit)
{
    m_point = point;
    m_suit = suit;
}

void Card::setPoint(CardPoint point)
{
    m_point = point;
}

void Card::setSuit(CardSuit suit)
{
    m_suit = suit;
}

Card::CardPoint Card::getPoint() const
{
    return m_point;
}

Card::CardSuit Card::getSuit() const
{
    return m_suit;
}

bool lessSort(const Card &c1, const Card &c2)
{
    if(c1.getPoint() == c2.getPoint())
    {
        return c1.getSuit() < c2.getSuit();
    }
    else
    {
        return c1.getPoint() < c2.getPoint();
    }
}

bool greaterSort(const Card &c1, const Card &c2)
{
    if(c1.getPoint() == c2.getPoint())
    {
        return c1.getSuit() < c2.getSuit();
    }
    else
    {
        return c1.getPoint() > c2.getPoint();
    }
}


bool operator ==(const Card& left, const Card& right)
{
    return (left.getPoint() == right.getPoint() && left.getSuit() == right.getSuit());
}

bool operator <(const Card& left, const Card& right)
{
    if(left.getPoint() == right.getPoint())
    {
        return left.getSuit() < right.getSuit();
    }
    else
    {
        return left.getPoint() < right.getPoint();
    }
}

uint qHash(const Card &card)
{
    return card.getPoint() * 100 + card.getSuit();
}


QDataStream& operator<<(QDataStream& in, const Card& card)
{
    int suit = static_cast<int>(card.getSuit());
    int point = static_cast<int>(card.getPoint());
    in << suit << point;
    return in;
}

QDataStream& operator>>(QDataStream& out, Card& card)
{
    int suit, point;
    out >> suit >> point;
    card.setSuit(static_cast<Card::CardSuit>(suit));
    card.setPoint(static_cast<Card::CardPoint>(point));
    return out;
}

