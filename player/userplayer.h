#ifndef USERPLAYER_H
#define USERPLAYER_H

#include <QObject>
#include "player.h"

class UserPlayer : public Player
{
    Q_OBJECT
public:
    using Player::Player;
    explicit UserPlayer(QObject *parent = nullptr);

signals:
    void startCountDown();

    // Player interface
public:
    virtual void prepareCallLord() override;
    virtual void preparePlayHand() override;
};



#endif // USERPLAYER_H
