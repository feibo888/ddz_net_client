#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H


#include "player.h"
#include <QRunnable>
#include <QThread>

class RobotGrapLord : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player, QObject *parent = nullptr);

signals:


private:
    Player* m_player;

    // QThread interface
protected:
    void run() override;


};



#endif // ROBOTGRAPLORD_H
