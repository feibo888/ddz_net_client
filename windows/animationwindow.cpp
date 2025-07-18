#include "animationwindow.h"


AnimationWindow::AnimationWindow(QWidget *parent)
    : QWidget{parent}
{}

void AnimationWindow::showBetScore(int bet)
{
    m_x = 0;
    if(bet == 1)
    {
        m_pixmap.load(":/images/score1.png");
    }
    else if(bet == 2)
    {
        m_pixmap.load(":/images/score2.png");
    }
    else if(bet == 3)
    {
        m_pixmap.load(":/images/score3.png");
    }
    update();

    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::showSequence(Type type)
{
    m_x = 0;
    QString name = type == Pair ? ":/images/liandui.png" : ":/images/shunzi.png";
    m_pixmap.load(name);

    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::showJokerBomb()
{
    m_x = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        m_index++;
        if(m_index > 8)
        {
            timer->stop();
            timer->deleteLater();
            m_index = 8;
            hide();
        }
        QString name = QString(":/images/joker_bomb_%1.png").arg(m_index);
        m_pixmap.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::showBomb()
{
    m_x = 0;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        m_index++;
        if(m_index > 12)
        {
            timer->stop();
            timer->deleteLater();
            m_index = 12;
            hide();
        }
        QString name = QString(":/images/bomb_%1.png").arg(m_index);
        m_pixmap.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::showPlane()
{
    m_x = width();
    m_pixmap.load(":/images/plane_1.png");
    setFixedHeight(m_pixmap.height());
    update();

    int step = width() / 5;
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        static int dist = 0;
        static int timers = 0;
        dist += 5;
        if(dist >= step)
        {
            dist = 0;
            timers++;
            QString name = QString(":/images/plane_%1.png").arg(timers % 5 + 1);
            m_pixmap.load(name);
        }
        if(m_x <= -110)
        {
            timer->stop();
            timer->deleteLater();
            dist = timers = 0;
            hide();
        }
        m_x -= 5;
        update();
    });
    timer->start(15);
}


void AnimationWindow::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(m_x, 0, m_pixmap.width(), m_pixmap.height(), m_pixmap);
}
