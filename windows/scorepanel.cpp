#include "scorepanel.h"
#include "ui_scorepanel.h"

const QString MYCOLOR[] = {"black", "white", "red", "blue", "green"};

ScorePanel::ScorePanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScorePanel)
{
    ui->setupUi(this);

    m_list << ui->meScore << ui->leftScore << ui->rightScore
           << ui->me << ui->left << ui->right
           << ui->me_fen << ui->left_fen << ui->right_fen;
}

ScorePanel::~ScorePanel()
{
    delete ui;
}

void ScorePanel::setScores(int left, int right, int user)
{
    ui->leftScore->setText(QString::number(left));
    ui->rightScore->setText(QString::number(right));
    ui->meScore->setText(QString::number(user));
}

void ScorePanel::setFontSize(int point)
{
    QFont font("Times", point, QFont::Bold);

    for(int i = 0; i < m_list.size(); ++i)
    {
        m_list[i]->setFont(font);
    }
}

void ScorePanel::setFontColor(FontColor color)
{
    QString style = QString("QLabel{color:%1;}").arg(MYCOLOR[color]);
    for(int i = 0; i < m_list.size(); ++i)
    {
        m_list[i]->setStyleSheet(style);
    }
}

void ScorePanel::setPLayerName(QByteArray left, QByteArray mid, QByteArray right)
{
    ui->left->setText(left);
    ui->me->setText(mid);
    ui->right->setText(right);
}
