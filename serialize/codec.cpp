#include "codec.h"

Codec::Codec(Message *msg)
{
    reload(msg);
}

Codec::Codec(QByteArray msg)
{
    reload(msg);
}

//将Message序列化为字符串
QByteArray Codec::enCodeMsg()
{
    std::string out;
    m_obj.SerializeToString(&out);
    return QByteArray::fromStdString(out);
}

//将字符串反序列化为Message
QSharedPointer<Message> Codec::deCodeMsg()
{
    std::string data = m_msg.toStdString();
    m_obj.ParseFromString(data);
    Message *msg = new Message;
    msg->userName = QByteArray::fromStdString(m_obj.username());
    msg->roomName = QByteArray::fromStdString(m_obj.roomname());
    msg->data1 = QByteArray::fromStdString(m_obj.data1());
    msg->data2 = QByteArray::fromStdString(m_obj.data2());
    msg->data3 = QByteArray::fromStdString(m_obj.data3());
    msg->reqCode = m_obj.reqcode();
    msg->resCode = m_obj.rescode();

    QSharedPointer<Message> ptr(msg, [this](Message* pt)
    {
        delete pt;
    });
    return ptr;
}

void Codec::reload(Message *msg)
{
    m_obj.set_username(msg->userName.toStdString());
    m_obj.set_roomname(msg->roomName.toStdString());
    m_obj.set_data1(msg->data1.toStdString());
    m_obj.set_data2(msg->data2.toStdString());
    m_obj.set_data3(msg->data3.toStdString());
    m_obj.set_reqcode(msg->reqCode);
    m_obj.set_rescode(msg->resCode);
}

void Codec::reload(QByteArray msg)
{
    m_msg = msg;
}
