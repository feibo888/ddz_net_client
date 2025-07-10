#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>

#include "card.h"
#include "player.h"

#include <QPainter>
#include <QMouseEvent>

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    CardPanel(QPixmap m_front, QPixmap m_back)
        : m_front(std::move(m_front)), m_back(std::move(m_back)) {}
    // 设置获取图片函数
    void setImage(const QPixmap& front, const QPixmap& back);
    QPixmap getImage();


    //扑克牌显示哪一面
    void setFrontSide(bool flag);
    bool isFrontSide();


    //记录窗口是否被选中
    void setSelected(bool flag);
    bool isSelected();

    //扑克牌的花色以及点数
    void setCard(const Card& card);
    Card getCard();

    //扑克牌的所有者
    void setOwner(Player* player);
    Player *getOwner();

    //模拟扑克牌的点击事件
    void clicked();


private:
    QPixmap m_front;
    QPixmap m_back;
    bool is_front = true;
    bool is_selected = false;
    Card m_card;
    Player* m_player = nullptr;


signals:
    void cardSelected(Qt::MouseButton button);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};



#endif // CARDPANEL_H
