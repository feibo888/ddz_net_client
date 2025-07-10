#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "userplayer.h"
#include "cards.h"
#include <QTimer>

struct BetRecord
{
    BetRecord()
    {
        reset();
    }

    void reset()
    {
        player = nullptr;
        bet = 0;
        times = 0;
    }

    Player* player;
    int bet;
    int times;
};


class GameControl : public QObject
{
    Q_OBJECT
public:

    //游戏状态
    enum GameStatus
    {
        DispatchCard,   //发牌
        CallingLord,    //叫地主状态
        PlayingHand     //出牌状态
    };

    //玩家状态
    enum PlayerStatus
    {
        ThinkingForCallLord,    //考虑叫地主
        ThinkingForPlayHand,    //考虑出牌
        Winning                 //赢
    };


    explicit GameControl(QObject *parent = nullptr);

    //玩家初始化
    void playerInit();

    //初始化扑克牌
    void cardsInit();

    //得到玩家的实例对象
    Robot* getLeftRobot();
    Robot* getRightRobot();
    UserPlayer* getUserPlayer();

    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer();

    //得到出牌玩家和打出的牌
    Player* getPendPlayer();
    Cards getPendCards();

    //初始化所有牌
    void initAllCards();

    //每次发一张牌
    Card takeOneCard();

    //得到最后的三张底牌
    Cards getSurplusCards();

    //重置卡牌数据
    void resetCardData();

    // 准备叫地主
    void startLordCard();

    //成为地主
    void becomeLord(Player* player, int bet);

    //清空所有玩家的得分
    void clearPlayerScore();

    //得到玩家下注的最高分数
    int getPlayerMaxBet();

    //处理叫地主
    void onGrabBet(Player* player, int bet);

    //处理出牌
    void onPlayHand(Player* player, Cards& cards);

    //设置当前玩家
    void setCurrentPlayer(int index);


private:
    Robot* m_robotLeft = nullptr;
    Robot* m_robotRight = nullptr;
    UserPlayer* m_user = nullptr;

    Player* m_currPlayer = nullptr;

    Player* m_pendPlayer = nullptr;
    Cards m_pendCards;

    Cards m_allCards;

    BetRecord m_record;

    int m_currBet = 0;

signals:
    void playerStatusChanged(Player* player, PlayerStatus status);
    //通知玩家抢地主
    void notifyGrabLordBet(Player* player, int bet, bool flag);
    //游戏状态变化
    void gameStatusChanged(GameStatus status);
    //通知玩家出牌
    void notifyPlayHand(Player* player, Cards& cards);
    //给玩家传递出牌数据
    void pendingInfo(Player* player, Cards& cards);
};

#endif // GAMECONTROL_H
