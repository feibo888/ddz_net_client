#ifndef LOGIN_H
#define LOGIN_H

#include <QLineEdit>
#include <QDialog>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QThreadPool>
#include <QCryptographicHash>
#include <QMessageBox>

#include "datamanager.h"
#include "codec.h"
#include "communication.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    //校验数据
    bool verifyData(QLineEdit* edit);
    //连接服务器
    void startConnection(Message* message);
    //槽函数
    void onLogin();
    void onRegister();
    void onNetOK();

    //保存数据
    void saveUserInfo();
    //加载数据
    void loadUserInfo();

private:
    Ui::Login *ui;
    bool m_isConnected = false;
    const QByteArray KEY = "当勾选记住密码选项时用于加密使用的KEY";
};

#endif // LOGIN_H
