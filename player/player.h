#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "card.h"
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum Role   //角色
    {
        Lord,
        Farmer
    };

    enum Sex    //性别
    {
        Man,
        Woman
    };

    enum Direction  //方向
    {
        Left,
        Right
    };

    enum Type       //玩家的类型
    {
        Robot,
        User,
        UnKnow
    };

    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject *parent = nullptr);

    //名字
    void setName(QString name);
    QString getName();

    //角色
    void setRole(Role role);
    Role getRole();

    //性别
    void setSex(Sex sex);
    Sex getSex();

    //方向
    void setDirection(Direction dir);
    Direction getDirection();

    //玩家类型
    void setType(Type type);
    Type getType();

    //玩家分数
    void setScore(int score);
    int getScore();

    //游戏结果
    void setWin(bool flag);
    bool getWin();

    //提供当前对象的上家/下家
    void setPrevPlayer(Player* player);
    void setNextPlayer(Player* player);
    Player* getPrevPlayer();
    Player* getNextPlayer();


    //叫地主/抢地主
    void grabLordBet(int point);

    //存储扑克牌（发牌时得到的）
    void storeDispatchCard(const Card& card);
    void storeDispatchCard(const Cards& cards);

    //得到所有牌
    Cards getCards();

    //清空所有牌
    void clearCards();

    //出牌
    void playHand(Cards& cards);

    void sethasPlayLeftCard2(bool flag);
    void sethasPlayLeftCard1(bool flag);

    bool gethasPlayLeftCard2();
    bool gethasPlayLeftCard1();

    //设置出牌的玩家和已经待处理的扑克牌
    void setPendingInfo(Player* player, const Cards& cards);
    Player* getPendPlayer();
    Cards getPendCards();

    //存储出牌对象和打出的牌
    void storePendingInfo(Player* player, const Cards& cards);

    //虚函数接口
    virtual void prepareCallLord();
    virtual void preparePlayHand();
    virtual void thinkCallLord();
    virtual void thinkPlayHand();

protected:
    int m_score = 0;
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_dir;
    Type m_type;
    bool m_isWin = false;
    Player* m_prev = nullptr;
    Player* m_next = nullptr;
    Cards m_cards;

    Cards m_pendCards;
    Player* m_pendPlayer = nullptr;
    bool m_hasPlayLeftCard2 = false;
    bool m_hasPlayLeftCard1 = false;

signals:
    //通知叫地主已经下注
    void notifyGrabLordBet(Player* player, int bet);

    //通知已经出牌
    void notifyPlayHand(Player* player, Cards& cards);

    //通知已经发牌了
    void notifyPickCards(Player* player, const Cards& cards);
};

#endif // PLAYER_H
