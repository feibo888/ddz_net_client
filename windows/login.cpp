#include "login.h"
#include "gamemode.h"
#include "ui_login.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>



Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    //setFixedSize(480, 350);
    setFixedSize(783, 506);

    ui->stackedWidget->setCurrentIndex(0);

    // 设置占位符文本
    ui->username->setPlaceholderText("3-16位字母、数字或下划线");
    ui->regUsername->setPlaceholderText("3-16位字母、数字或下划线");
    
    ui->password->setPlaceholderText("4-20位，须包含大小写字母、数字和特殊字符");
    ui->regPassword->setPlaceholderText("4-20位，须包含大小写字母、数字和特殊字符");
    
    ui->regPhone->setPlaceholderText("以1开头的11位数字");
    ui->IPAddr->setPlaceholderText("如 192.168.1.1");
    ui->Port->setPlaceholderText("端口号: 0-65535");

    // 设置密码模式
    ui->password->setEchoMode(QLineEdit::Password);
    ui->regPassword->setEchoMode(QLineEdit::Password);

    // 创建显示/隐藏密码的按钮
    QPushButton* showPwdBtn = new QPushButton(this);
    showPwdBtn->setText("👁");
    showPwdBtn->setFixedSize(30, 25);
    showPwdBtn->setStyleSheet("border: none; background: transparent; font-size: 14px;");
    showPwdBtn->setCursor(Qt::PointingHandCursor);
    
    QPushButton* showRegPwdBtn = new QPushButton(this);
    showRegPwdBtn->setText("👁");
    showRegPwdBtn->setFixedSize(30, 25);
    showRegPwdBtn->setStyleSheet("border: none; background: transparent; font-size: 14px;");

    // 将按钮放置在密码输入框内部
    QHBoxLayout* pwdLayout = new QHBoxLayout(ui->password);
    pwdLayout->addStretch();
    pwdLayout->addWidget(showPwdBtn);
    pwdLayout->setContentsMargins(0, 0, 5, 0);
    ui->password->setLayout(pwdLayout);

    QHBoxLayout* regPwdLayout = new QHBoxLayout(ui->regPassword);
    regPwdLayout->addStretch();
    regPwdLayout->addWidget(showRegPwdBtn);
    regPwdLayout->setContentsMargins(0, 0, 5, 0);
    ui->regPassword->setLayout(regPwdLayout);

    // 连接密码显示切换功能
    connect(showPwdBtn, &QPushButton::clicked, this, [=]() {
        if (ui->password->echoMode() == QLineEdit::Password) {
            ui->password->setEchoMode(QLineEdit::Normal);
            showPwdBtn->setText("🙈");
        } else {
            ui->password->setEchoMode(QLineEdit::Password);
            showPwdBtn->setText("👁");
        }
    });

    connect(showRegPwdBtn, &QPushButton::clicked, this, [=]() {
        if (ui->regPassword->echoMode() == QLineEdit::Password) {
            ui->regPassword->setEchoMode(QLineEdit::Normal);
            showRegPwdBtn->setText("🙈");
        } else {
            ui->regPassword->setEchoMode(QLineEdit::Password);
            showRegPwdBtn->setText("👁");
        }
    });

    connect(ui->home, &QPushButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->registerInLogin, &QPushButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->netSet, &QPushButton::clicked, this, [=]()
    {
        ui->stackedWidget->setCurrentIndex(2);
    });

    //用户名校验
    QRegularExpression expreg("^[a-zA-Z0-9_]{3,16}$");
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator(expreg, this);
    ui->username->setValidator(val1);
    ui->regUsername->setValidator(val1);

    //密码校验，至少一个大写，一个小写，一个数字，一个特殊字符，长度4-20
    expreg.setPattern("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{4,20}$");
    QRegularExpressionValidator *val2 = new QRegularExpressionValidator(expreg, this);
    ui->password->setValidator(val2);
    ui->regPassword->setValidator(val2);

    //手机号
    expreg.setPattern("^1[3456789]\\d{9}$");
    QRegularExpressionValidator *val3 = new QRegularExpressionValidator(expreg, this);
    ui->regPhone->setValidator(val3);

    // IP校验
    expreg.setPattern("^((\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.){3}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    QRegularExpressionValidator *val4 = new QRegularExpressionValidator(expreg, this);
    ui->IPAddr->setValidator(val4);

    // 端口校验
    expreg.setPattern("^(0|([1-9]\\d{0,3})|([1-5]\\d{4})|(6[0-4]\\d{3})|(65[0-4]\\d{2})|(655[0-2]\\d)|(6553[0-5]))$");
    QRegularExpressionValidator *val5 = new QRegularExpressionValidator(expreg, this);
    ui->Port->setValidator(val5);


    connect(ui->loginBtn, &QPushButton::clicked, this, &Login::onLogin);
    connect(ui->registerBtn, &QPushButton::clicked, this, &Login::onRegister);
    connect(ui->netOKBtn, &QPushButton::clicked, this, &Login::onNetOK);

    QThreadPool::globalInstance()->setMaxThreadCount(8);

    //test code
    // ui->username->setText("admin");
    // ui->password->setText("1aA@");

    loadUserInfo();
}

Login::~Login()
{
    delete ui;
}

bool Login::verifyData(QLineEdit* edit)
{
    if(edit->hasAcceptableInput() == false)
    {
        edit->setStyleSheet("border: 2px solid red;");
        return false;
    }
    else
    {
        edit->setStyleSheet("none");
    }
    return true;
}

void Login::startConnection(Message *message)
{
    if(!m_isConnected)
    {
        Communication* task = new Communication(*message);
        connect(task, &Communication::connectFailed, this, [=]()
        {
            QMessageBox::critical(this, "连接服务器", "连接服务器失败");
            m_isConnected = false;
        });

        connect(task, &Communication::loginOk, this, [=]()
        {
            //将用户名保存到单例对象
            DataManager::getInstance()->setUserName(ui->username->text().toUtf8());
            //保存用户名密码
            saveUserInfo();
            //显示游戏窗口
            GameMode* mode = new GameMode;
            mode->show();
            accept();
        });

        connect(task, &Communication::registerOk, this, [=]()
        {
            ui->stackedWidget->setCurrentIndex(0);
        });

        connect(task, &Communication::failedMsg, this, [=](QByteArray msg)
        {
            QMessageBox::critical(this, "ERROR", msg);
        });


        m_isConnected = true;

        QThreadPool::globalInstance()->start(task);
        DataManager::getInstance()->setCommunication(task);

    }
    else
    {
        //和服务器通信
        DataManager::getInstance()->getCommunication()->sendMessage(message);
    }

}

void Login::onLogin()
{
    bool flag1 = verifyData(ui->username);
    bool flag2 = verifyData(ui->password);
    if(flag1 && flag2)
    {
        Message msg;
        msg.userName = ui->username->text().toUtf8();
        msg.reqCode = RequestCode::UserLogin;

        QByteArray passwd = ui->password->text().toUtf8();
        passwd = QCryptographicHash::hash(passwd, QCryptographicHash::Sha224).toHex();
        msg.data1 = passwd;

        //连接服务器
        startConnection(&msg);
    }

}

void Login::onRegister()
{
    bool flag1 = verifyData(ui->regUsername);
    bool flag2 = verifyData(ui->regPassword);
    bool flag3 = verifyData(ui->regPhone);
    if(flag1 && flag2 && flag3)
    {
        Message msg;
        msg.userName = ui->regUsername->text().toUtf8();
        msg.reqCode = RequestCode::Register;

        QByteArray passwd = ui->regPassword->text().toUtf8();
        passwd = QCryptographicHash::hash(passwd, QCryptographicHash::Sha224).toHex();
        msg.data1 = passwd;

        msg.data2 = ui->regPhone->text().toUtf8();
        //连接服务器
        startConnection(&msg);
    }
}

void Login::onNetOK()
{
    bool flag1 = verifyData(ui->IPAddr);
    bool flag2 = verifyData(ui->Port);
    if(flag1 && flag2)
    {
        DataManager* data = DataManager::getInstance();
        data->setIP(ui->IPAddr->text().toUtf8());
        data->setPort(ui->Port->text().toUtf8());

        ui->stackedWidget->setCurrentIndex(0);
    }
}

void Login::saveUserInfo()
{
    if(ui->savePwd->isChecked())
    {
        QJsonObject obj;
        obj.insert("user", ui->username->text());
        obj.insert("passwd", ui->password->text());
        QJsonDocument doc(obj);
        QByteArray json = doc.toJson();
        //aes加密
        AesCrypto aes(AesCrypto::AES_CBC_128, KEY.left(16));
        json = aes.enctypt(json);
        //写文件
        QFile file("passwd.bin");
        file.open(QFile::WriteOnly);
        file.write(json);
        file.close();
    }
    else
    {
        QFile file("passwd.bin");
        file.remove();
    }
}

void Login::loadUserInfo()
{
    QFile file("passwd.bin");
    bool flag = file.open(QFile::ReadOnly);
    if(flag)
    {
        ui->savePwd->setChecked(true);
        QByteArray all = file.readAll();
        //aes解密
        AesCrypto aes(AesCrypto::AES_CBC_128, KEY.left(16));
        all = aes.dectypt(all);
        //转为json对象
        QJsonDocument doc = QJsonDocument::fromJson(all);
        QJsonObject obj = doc.object();

        QString name = obj.value("user").toString();
        QString passwd = obj.value("passwd").toString();
        ui->username->setText(name);
        ui->password->setText(passwd);
    }
    else
    {
        ui->savePwd->setChecked(false);
    }

}
