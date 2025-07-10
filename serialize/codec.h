#ifndef CODEC_H
#define CODEC_H

#include "infomation.pb.h"
#include <QByteArray>
#include <QSharedPointer>

struct Message
{
    QByteArray userName;
    QByteArray roomName;
    QByteArray data1;
    QByteArray data2;
    QByteArray data3;
    RequestCode reqCode;
    ResponseCode resCode;
};



class Codec
{
public:
    //序列化
    Codec(Message* msg);
    //反序列化
    Codec(QByteArray msg);
    //数据编码
    QByteArray enCodeMsg();
    //数据解码
    QSharedPointer<Message> deCodeMsg();
    //重新加载数据
    void reload(Message* msg);
    void reload(QByteArray msg);

private:
    QByteArray m_msg;
    Information m_obj;

};

#endif // CODEC_H
