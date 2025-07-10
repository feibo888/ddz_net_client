#include "cardpanel.h"


CardPanel::CardPanel(QWidget *parent)
    : QWidget{parent}
{
}

void CardPanel::setImage(const QPixmap &front, const QPixmap &back)
{
    m_front = front;
    m_back = back;

    setFixedSize(m_front.size());

    update();

}

QPixmap CardPanel::getImage()
{
    return m_front;
}

void CardPanel::setFrontSide(bool flag)
{
    is_front = flag;
}

bool CardPanel::isFrontSide()
{
    return is_front;
}

void CardPanel::setSelected(bool flag)
{
    is_selected = flag;
}

bool CardPanel::isSelected()
{
    return is_selected;
}

void CardPanel::setCard(const Card &card)
{
    m_card = card;
}

Card CardPanel::getCard()
{
    return m_card;
}

void CardPanel::setOwner(Player *player)
{
    m_player = player;
}

Player *CardPanel::getOwner()
{
    return m_player;
}

void CardPanel::clicked()
{
    emit cardSelected(Qt::LeftButton);
}

void CardPanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    if(is_front)
    {
        p.drawPixmap(rect(), m_front);
    }
    else
    {
        p.drawPixmap(rect(), m_back);
    }
}


void CardPanel::mousePressEvent(QMouseEvent *event)
{
    emit cardSelected(event->button());
}
