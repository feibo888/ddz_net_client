#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <QObject>
#include <QByteArray>
#include <QCryptographicHash>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <QMap>
#include "base64.h"

class RsaCrypto : public QObject
{
    Q_OBJECT
public:

    enum KeyLength
    {
        Bits_1K = 1024,
        Bits_2K = 2048,
        Bits_3K = 3072,
        Bits_4K = 4096
    };
    enum KeyType{PublicKey, PrivateKey};

    using hashFunc = const EVP_MD *(*)(void);

    explicit RsaCrypto(QObject *parent = nullptr);

    //构造对象并加载密钥文件中的数据到内存中
    explicit RsaCrypto(QByteArray fileName, KeyType type, QObject *parent = nullptr);
    ~RsaCrypto();

    //将密钥字符串解析为密钥类型
    void parseStringToKey(QByteArray data, KeyType type);
    //生成密钥对
    void generateRsaKey(KeyLength bits, QByteArray pub = "public.pem", QByteArray pri = "private.pem");

    //通过公钥进行加密
    QByteArray pubKeyEncrypt(QByteArray data);
    //通过私钥进行解密
    QByteArray priKeyDecrypt(QByteArray data);

    //数据签名
    QByteArray sign(QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);
    //签名校验
    bool verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash = QCryptographicHash::Sha256);


signals:

private:
    EVP_PKEY *m_pubKey = NULL;
    EVP_PKEY *m_priKey = NULL;

    const QMap<QCryptographicHash::Algorithm, hashFunc> m_hashMethod =
    {
        {QCryptographicHash::Md5, EVP_md5},
        {QCryptographicHash::Sha1, EVP_sha1},
        {QCryptographicHash::Sha224, EVP_sha224},
        {QCryptographicHash::Sha256, EVP_sha256},
        {QCryptographicHash::Sha384, EVP_sha384},
        {QCryptographicHash::Sha512, EVP_sha512},
        {QCryptographicHash::Sha3_224, EVP_sha3_224},
        {QCryptographicHash::Sha3_256, EVP_sha3_256},
        {QCryptographicHash::Sha3_384, EVP_sha3_384},
        {QCryptographicHash::Sha3_512, EVP_sha3_512}
    };
};

#endif // RSACRYPTO_H
