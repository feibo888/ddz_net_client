#ifndef BASE64_H
#define BASE64_H

#include <QObject>
#include <QByteArray>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

class Base64 : public QObject
{
    Q_OBJECT
public:
    explicit Base64(QObject *parent = nullptr);

    //编码
    QByteArray encode(QByteArray data);
    QByteArray encode(const char* data, int size);
    //解码
    QByteArray decode(QByteArray data);
    QByteArray decode(const char* data, int size);

signals:
};

#endif // BASE64_H
