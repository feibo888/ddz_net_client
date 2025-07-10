#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

    void setImage(QString normal, QString hover, QString pressed, QString disable = QString());
    void setBtnDisable(bool flag);

signals:


private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QString m_disable;
    QPixmap m_pixmap;
    bool m_isDisable = false;


    // QWidget interface
protected:
    //鼠标按下，离开，进入按钮，绘画
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void leaveEvent(QEvent *event) override;




};



#endif // MYBUTTON_H
