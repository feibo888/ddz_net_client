#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

class CountDown : public QWidget
{
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);

    void showCountDown();
    void stopCountDown();

signals:
    void notMuchTime();
    void timeout();

private:
    QPixmap m_clock;
    QPixmap m_number;
    QTimer* m_timer;
    int m_count;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};



#endif // COUNTDOWN_H
