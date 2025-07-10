#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(Cards &cards)
{
    //分类
    classify(cards);
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra)
{
    m_type = type;
    m_pt = pt;
    m_extra = extra;
}

void PlayHand::classify(Cards &cards)
{
    CardList list = cards.toCardList();
    QVector<int> cardRecord(Card::Card_End, 0);

    for(int i = 0; i < list.size(); ++i)
    {
        Card c = list.at(i);
        cardRecord[c.getPoint()]++;
    }

    m_oneCard.clear();
    m_twoCard.clear();
    m_threeCard.clear();
    m_fourCard.clear();

    for(int i = 0; i < Card::Card_End; ++i)
    {
        if(cardRecord[i] == 1)
        {
            m_oneCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i] == 2)
        {
            m_twoCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i] == 3)
        {
            m_threeCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i] == 4)
        {
            m_fourCard.push_back((Card::CardPoint)i);
        }
    }

}

void PlayHand::judgeCardType()
{
    m_type = HandType::Hand_Unknown;
    m_pt = Card::Card_Begin;
    m_extra = 0;

    if(isPass())
    {
        m_type = Hand_Pass;
    }
    else if(isSingle())
    {
        m_type = Hand_Single;
        m_pt = m_oneCard[0];
    }
    else if(isPair())
    {
        m_type = Hand_Pair;
        m_pt = m_twoCard[0];
    }
    else if(isTriple())
    {
        m_type = Hand_Triple;
        m_pt = m_threeCard[0];
    }
    else if(isTripleSingle())
    {
        m_type = Hand_Triple_Single;
        m_pt = m_threeCard[0];
    }
    else if(isTriplePair())
    {
        m_type = Hand_Triple_Pair;
        m_pt = m_threeCard[0];
    }
    else if(isPlane())
    {
        m_type = Hand_Plane;
        m_pt = m_threeCard[0];
    }
    else if(isPlaneTwoSingle())
    {
        m_type = Hand_Plane_Two_Single;
        m_pt = m_threeCard[0];
    }
    else if(isPlaneTwoPair())
    {
        m_type = Hand_Plane_Two_Pair;
        m_pt = m_threeCard[0];
    }
    else if(isSeqPair())
    {
        m_type = Hand_Seq_Pair;
        m_pt = m_twoCard[0];
        m_extra = m_twoCard.size();
    }
    else if(isSeqSingle())
    {
        m_type = Hand_Seq_Single;
        m_pt = m_oneCard[0];
        m_extra = m_oneCard.size();
    }
    else if(isBomb())
    {
        m_type = Hand_Bomb;
        m_pt = m_fourCard[0];
    }
    else if(isBombTwoSingle())
    {
        m_type = Hand_Bomb_Two_Single;
        m_pt = m_fourCard[0];
    }
    else if(isBombTwoPair())
    {
        m_type = Hand_Bomb_Two_Pair;
        m_pt = m_fourCard[0];
    }
    else if(isBombJokers())
    {
        m_type = Hand_Bomb_Jokers;
    }
}

bool PlayHand::isPass()
{
    if(m_oneCard.empty() && m_twoCard.empty() && m_threeCard.empty() && m_fourCard.empty())
    {
        return true;
    }
    return false;
}

PlayHand::HandType PlayHand::getHandType()
{
    return m_type;
}

Card::CardPoint PlayHand::getCardPoint()
{
    return m_pt;
}

int PlayHand::getExtra()
{
    return m_extra;
}

bool PlayHand::canBeat(const PlayHand &other)
{
    //我的牌型未知
    if(m_type == Hand_Unknown)
    {
        return false;
    }
    //对方放弃出牌
    if(other.m_type == Hand_Pass)
    {
        return true;
    }
    //我是王炸
    if(m_type == Hand_Bomb_Jokers)
    {
        return true;
    }

    if(m_type == Hand_Bomb && other.m_type < Hand_Bomb)
    {
        return true;
    }

    //牌型一致
    if(m_type == other.m_type)
    {
        //连队和顺子
        if(m_type == Hand_Seq_Pair || m_type == Hand_Seq_Single)
        {
            return m_pt > other.m_pt && m_extra == other.m_extra;
        }
        else
        {
            return m_pt > other.m_pt;
        }
    }
    return false;

}

bool PlayHand::isSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.empty() && m_threeCard.empty() && m_fourCard.empty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPair()
{
    if(m_oneCard.empty() && m_twoCard.size() == 1 && m_threeCard.empty() && m_fourCard.empty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriple()
{
    if(m_oneCard.empty() && m_twoCard.empty() && m_threeCard.size() == 1 && m_fourCard.empty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTripleSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.empty() && m_threeCard.size() == 1 && m_fourCard.empty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriplePair()
{
    if(m_oneCard.empty() && m_twoCard.size() == 1 && m_threeCard.size() == 1 && m_fourCard.empty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPlane()
{
    if(m_oneCard.empty() && m_twoCard.empty() && m_threeCard.size() == 2 && m_fourCard.empty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle()
{
    if((m_oneCard.size() == 2 && m_twoCard.empty() && m_threeCard.size() == 2 && m_fourCard.empty()) ||
        (m_oneCard.empty() && m_twoCard.size() == 1 && m_threeCard.size() == 2 && m_fourCard.empty()))
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if(m_oneCard.empty() && m_twoCard.size() == 2 && m_threeCard.size() == 2 && m_fourCard.empty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if(m_oneCard.empty() && m_twoCard.size() >= 3 && m_threeCard.empty() && m_fourCard.empty())
    {
        std::sort(m_twoCard.begin(), m_twoCard.end());
        if(m_twoCard.last() - m_twoCard.first() == (m_twoCard.size() - 1) &&
            m_twoCard.first() >= Card::Card_3 && m_twoCard.last() < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle()
{
    if(m_oneCard.size() >= 5 && m_twoCard.empty() && m_threeCard.empty() && m_fourCard.empty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.last() - m_oneCard.first() == (m_oneCard.size() - 1) &&
            m_oneCard.first() >= Card::Card_3 && m_oneCard.last() < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb()
{
    if(m_oneCard.empty() && m_twoCard.empty() && m_threeCard.empty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle()
{
    if((m_oneCard.size() == 2 && m_twoCard.empty() && m_threeCard.empty() && m_fourCard.size() == 1) ||
        (m_oneCard.empty() && m_twoCard.size() == 1 && m_threeCard.empty() && m_fourCard.size() == 1))
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombTwoPair()
{
    if(m_oneCard.empty() && m_twoCard.size() == 2 && m_threeCard.empty() && m_fourCard.size() == 1)
    {
        return true;
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if(m_oneCard.size() == 2 && m_twoCard.empty() && m_threeCard.empty() && m_fourCard.empty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.first() == Card::Card_SJ && m_oneCard.last() == Card::Card_BJ)
        {
            return true;
        }
    }
    return false;
}




