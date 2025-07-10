#ifndef LOADING_H
#define LOADING_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include "login.h"

class Loading : public QWidget
{
    Q_OBJECT
public:
    explicit Loading(QWidget *parent = nullptr);

signals:
    void loadingFinished();

private:
    QPixmap m_bk;
    QPixmap m_progress;
    int m_dist = 15;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif // LOADING_H
