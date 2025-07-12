#include "communication.h"

#include <RsaCrypto.h>
#include <QDateTime>
#include <QDataStream>
#include <QThreadPool>
#include "cards.h"
#include "card.h"
#include "datamanager.h"
#include "taskqueue.h"


Communication::Communication(Message& msg, QObject *parent)
    : QObject{parent}, QRunnable(), m_msgInfo(msg)
{
    setAutoDelete(true);
}

Communication::~Communication()
{
    if(m_aes)
    {
        delete m_aes;
    }
}

void Communication::run()
{
    m_socket = new TcpSocket;
    DataManager* obj = DataManager::getInstance();
    bool flag = m_socket->connectToHost(obj->getIP(), obj->getPort().toInt());
    qDebug() << "===== 通信的子线程ID：" << QThread::currentThread() << " connected ? " << flag << Qt::endl;

    if(!flag)
    {
        emit connectFailed();
        delete m_socket;
        return;
    }

    //连接成功
    while(!m_stop)
    {
        //处理服务器回复的数据
        parseRecvMessage();

        // 添加短暂休眠，避免CPU占用过高，同时允许及时响应停止信号
        QThread::msleep(10);

    }
    m_socket->disConnect();
    delete m_socket;
    m_socket = nullptr;

    qDebug() << "Communication thread finished" << Qt::endl;
}

void Communication::sendMessage(Message *msg, bool encrypt)
{
    Codec codec(msg);
    QByteArray data = codec.enCodeMsg();
    //加密数据
    if(encrypt)
    {
        data = m_aes->enctypt(data);
    }

    m_socket->sendMsg(data);

}

void Communication::parseRecvMessage()
{
    //接收数据
    QByteArray data = m_socket->recvMsg(2);
    if(data.isEmpty())
    {
        return;
    }

    //反序列化数据
    Codec codec(data);
    QSharedPointer<Message> ptr = codec.deCodeMsg();

    qDebug() << ptr->resCode << ", " << ptr->data1 << ", " << ptr->data2 << Qt::endl;
    switch(ptr->resCode)
    {
    case ResponseCode::LoginOk:
        emit loginOk();
        qDebug() << "登录成功" << Qt::endl;
        break;
    case ResponseCode::RegisterOk:
        qDebug() << "注册成功" << Qt::endl;
        emit registerOk();
        break;
    case ResponseCode::RsaFenFa:
        handleRsaFenFa(ptr.get());
        break;
    case ResponseCode::AesVerifyOk:
        m_aes = new AesCrypto(AesCrypto::AES_CBC_256, m_aesKey);
        sendMessage(&m_msgInfo);
        qDebug() << "Aes密钥分发成功.." << Qt::endl;
        break;
    case ResponseCode::Failed:
        emit failedMsg(ptr->data1);
        break;
    case ResponseCode::JoinRoomOK:
        DataManager::getInstance()->setRoomName(ptr->roomName);
        emit playerCount(ptr->data1.toInt());
        break;
    case ResponseCode::DealCards:
        parseCards(ptr->data1, ptr->data2);
        break;
    case ResponseCode::StartGame:
        emit startGame(ptr->data1);
        break;
    case ResponseCode::OtherGrabLord:
    {
        Task t;
        t.bet = ptr->data1.toInt();
        TaskQueue::getInstance()->add(t);
        break;
    }
    case ResponseCode::OtherPlayHand:
    {
        Task t;
        QDataStream stream(ptr->data2);
        //data1 数据，data2数量
        for(int i = 0; i < ptr->data1.toInt(); ++i)
        {
            Card c;
            stream >> c;
            t.cards.add(c);
        }
        TaskQueue::getInstance()->add(t);
        break;
    }
    case ResponseCode::SearchRoomOK:
    {
        bool flag = ptr->data1 == "true" ? true : false;
        emit roomExist(flag);
        break;
    }
    case ResponseCode::OtherLeaveRoom:
    {
        int count = ptr->data1.toInt();
        emit somebodyLeave(count);
        break;
    }
    default:
        break;
    }

}

void Communication::handleRsaFenFa(Message *msg)
{
    RsaCrypto rsa;
    rsa.parseStringToKey(msg->data1, RsaCrypto::KeyType::PublicKey);
    //校验签名
    bool flag = rsa.verify(msg->data2, msg->data1);
    assert(flag);

    //生成对称加密密钥
    m_aesKey = generateAesKey(KeyLen::L32);
    Message resMsg;
    resMsg.reqCode = AesFenFa;
    resMsg.data1 = rsa.pubKeyEncrypt(m_aesKey);
    resMsg.data2 = QCryptographicHash::hash(m_aesKey, QCryptographicHash::Sha224).toHex();
    sendMessage(&resMsg, false);
}

QByteArray Communication::generateAesKey(KeyLen len)
{
    QByteArray time = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh::mm:ss.zzz").toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha3_384);
    hash.addData(time);
    time = hash.result();
    time = time.left(len);
    return time;
}


void Communication::parseCards(QByteArray data1, QByteArray data2)
{
    auto func = bind([=](QByteArray msg)
    {
        auto lst = msg.left(msg.size() - 1).split('#');
        Cards cards;
        for(const auto& it : lst)
        {
            auto sub = it.split('-');
            Card card(static_cast<Card::CardPoint>(sub.last().toInt()),
                      static_cast<Card::CardSuit>(sub.first().toInt()));
            cards.add(card);
        }
        return cards;
    }, std::placeholders::_1);

    Cards cards = func(data1);
    Cards last = func(data2);
    DataManager::getInstance()->setCards(cards, last);
}
