#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor
    {
        Black,
        White,
        Red,
        Blue,
        Green
    };

    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    void setScores(int left, int right, int user);

    //设置字体大小
    void setFontSize(int point);

    //设置字体颜色
    void setFontColor(FontColor color);

    //设置玩家名字
    void setPLayerName(QByteArray left, QByteArray mid, QByteArray right);

private:
    Ui::ScorePanel *ui;
    QVector<QLabel*> m_list;
};

#endif // SCOREPANEL_H
