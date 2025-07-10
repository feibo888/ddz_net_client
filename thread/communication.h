#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>
#include "tcpsocket.h"

#include "codec.h"
#include <QThread>
#include <QDebug>
#include "aescrypto.h"


class Communication : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum KeyLen{L16 = 16, L24 = 24, L32 = 32};

    explicit Communication(Message& msg, QObject *parent = nullptr);
    ~Communication();

    void run();
    inline void setStop()
    {
        m_stop = true;
    }

    //给服务器发送数据
    void sendMessage(Message* msg, bool encrypt = true);
    //解析服务器发来的数据
    void parseRecvMessage();
    //处理RsaFenFa
    void handleRsaFenFa(Message* msg);
    //生成对称加密密钥
    QByteArray generateAesKey(KeyLen len);
    //解析牌数据
    void extracted();
    void parseCards(QByteArray data1, QByteArray data2);

signals:
    void connectFailed();
    void loginOk();
    void registerOk();
    void failedMsg(QByteArray msg);
    void playerCount(int num);
    void startGame(QByteArray msg);
    void roomExist(bool flag);
    void somebodyLeave(int count);

private:
    TcpSocket* m_socket = nullptr;
    bool m_stop = false;
    Message m_msgInfo;
    QByteArray m_aesKey;
    AesCrypto* m_aes = nullptr;
};

#endif // COMMUNICATION_H
