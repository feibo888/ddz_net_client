#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include <QMainWindow>
#include <QPainter>
#include <QRandomGenerator>
#include "cardpanel.h"
#include "gamecontrol.h"
#include <QMap>
#include <QLabel>
#include <QTimer>
#include "animationwindow.h"
#include "endingpanel.h"
#include <QPropertyAnimation>
#include "countdown.h"
#include "bgmcontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:

    enum AnimationType
    {
        ShunZi,
        LianDui,
        Plane,
        JokerBomb,
        Bomb,
        Bet
    };


    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    //初始化游戏控制类对象
    void GameControlInit();

    //更新玩家分数
    void updatePlayersScore();

    //切割并存储图片
    void initCardMap();

    //切割图片
    void cropImage(QPixmap& pix, int x, int y, Card& c);

    //初始化按钮组
    void initButtonGroup();

    //初始化玩家在窗口的上下文环境
    void initPlayerContext();

    //初始化游戏场景
    void initGameScene();

    //初始化闹钟倒计时
    void initCountDown();

    //处理游戏的状态
    void gameStatusPrecess(GameControl::GameStatus status);

    //发牌
    void startDispatchCard();

    //移动扑克牌
    void cardMoveStep(Player* player, int curPos);

    //处理分发得到的扑克牌
    void disposCard(Player* player, const Cards& cards);

    //更新扑克牌在窗口的显示
    void updatePlayerCards(Player* player);

    //处理玩家状态的变化
    void onplayerStatusChanged(Player* player, GameControl::PlayerStatus status);

    //处理玩家抢地主
    void onGrabLordBet(Player* player, int bet, bool flag);

    //处理玩家出的牌
    void onDisposePlayHand(Player* player, Cards& cards);

    //定时器处理
    void onDispatchCard();

    //处理玩家选派
    void onCardSelected(Qt::MouseButton button);

    //处理用户玩家出牌
    void onUserPlayHand();

    //处理用户不出牌
    void onUserPass();

    //显示特效动画
    void showAnimation(AnimationType type, int bet = 0);

    //隐藏玩家打出的牌
    void hidePlayerDropCards(Player* player);

    //加载玩家头像
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role);

    //显示玩家的最终得分
    void showEndingScorePanel();

    using orderMap = QMap<int, QPair<QByteArray, int>>;
    //网络模式下初始化
    void initGamePanel(QByteArray msg);
    //更新用户名和分数
    void updatePlayerInfo(orderMap& info);
    void startGame();
    void startGame(int index);
    //通知其他玩家出牌
    void notifyOtherPlayHand(Cards& cs);

private:
    enum CardAlign
    {
        Horizontal,
        Vertical
    };

    struct PlayerContext
    {
        //1. 玩家扑克牌现实的区域
        QRect cardRect;
        //2. 出牌的区域
        QRect playHandRect;
        //3. 扑克牌的对齐方式（水平 or 垂直）
        CardAlign align;
        //4. 扑克牌显示正面还是背面
        bool isFrontSide;
        //5. 游戏过程中的提示信息，比如：不出
        QLabel* info;
        //6. 玩家的头像
        QLabel* roleImg;
        //7. 玩家刚打出的牌
        Cards lastCards;

    };


    Ui::GamePanel *ui;
    QPixmap m_bkImage;      //背景图片
    GameControl* m_gameCtl;     //游戏控制类
    QVector<Player*> m_playerList;      //三个玩家
    QMap<Card, CardPanel*> m_cardMap;       //每张牌对应的CardPanel
    QSize m_cardSize;   //每张牌的大小
    QPixmap m_cardbackimg;  //牌的背面图片
    QMap<Player*, PlayerContext> m_contextMap;  //玩家和他对应的区域

    CardPanel* m_baseCard;      //发牌区的牌
    CardPanel* m_moveCard;      //发牌时移动的牌
    QVector<CardPanel*> m_last3Cards;   //最后三张牌

    QPoint m_baseCardPos;       //发牌时牌的位置
    GameControl::GameStatus m_gameStatus;   //当前游戏状态

    QTimer* m_timer;

    AnimationWindow* m_animation;

    //点选
    CardPanel* m_curSelCard;
    QSet<CardPanel*> m_selectCards;

    //范围选
    QRect m_cardsRect;      //牌所在区域
    QHash<CardPanel*, QRect> m_userCards;   //每张牌对应的区域

    CountDown* m_countDown;

    BGMControl* m_bgm;

    QByteArrayList m_nameList;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

signals:
    void panelClose();

};






#endif // GAMEPANEL_H
