#include "mybutton.h"

MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{}

void MyButton::setImage(QString normal, QString hover, QString pressed, QString disable)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;
    m_disable = disable;
    m_pixmap.load(m_normal);
}

void MyButton::setBtnDisable(bool flag)
{
    m_isDisable = flag;
    setDisabled(flag);
    QString pic = flag ? m_disable : m_normal;
    m_pixmap.load(pic);
    update();
}

void MyButton::mousePressEvent(QMouseEvent *event)
{
    if(m_isDisable)
    {
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_isDisable)
    {
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void MyButton::enterEvent(QEvent *event)
{
    if(m_isDisable)
    {
        return;
    }
    m_pixmap.load(m_hover);
    update();
}

void MyButton::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
}

void MyButton::leaveEvent(QEvent *event)
{
    m_pixmap.load(m_normal);
    update();
}
