#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include "scorepanel.h"
#include <QPushButton>

class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);

    void setPlayerScore(int left, int right, int me);

    void setPlayName(QByteArrayList names);

signals:
    void continueGame();

private:
    QPixmap m_bk;
    QLabel* m_title;
    ScorePanel* m_score;
    QPushButton* m_continue;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif // ENDINGPANEL_H
