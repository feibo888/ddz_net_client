#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QThread>
#include <QRunnable>
#include "player.h"

class RobotPlayHand : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Player* player, QObject *parent = nullptr);

signals:



private:
    Player* m_player;

    // QThread interface
protected:
    void run() override;
};



#endif // ROBOTPLAYHAND_H
