#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QThreadPool>
#include "player.h"
#include "strategy.h"
#include "robotgraplord.h"
#include "robotplayhand.h"

class Robot : public Player
{
    Q_OBJECT
public:
    using Player::Player;
    explicit Robot(QObject *parent = nullptr);



    // Player interface
public:
    void prepareCallLord() override;
    void preparePlayHand() override;

    //卡考虑叫地主
    void thinkCallLord() override;

    //考虑出牌
    void thinkPlayHand() override;
};




#endif // ROBOT_H
