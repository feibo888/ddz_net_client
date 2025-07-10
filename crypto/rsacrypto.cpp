#include "rsacrypto.h"

RsaCrypto::RsaCrypto(QObject *parent)
    : QObject{parent}
{}

RsaCrypto::RsaCrypto(QByteArray fileName, KeyType type, QObject *parent)
{
    BIO *bio = BIO_new_file(fileName.data(), "rb");
    assert(bio != NULL);

    if(type == PublicKey)
    {
        PEM_read_bio_PUBKEY(bio, &m_pubKey, NULL, NULL);
    }
    else
    {
        PEM_read_bio_PrivateKey(bio, &m_priKey, NULL, NULL);
    }
    BIO_free(bio);
}

RsaCrypto::~RsaCrypto()
{
    if(m_pubKey)
    {
        EVP_PKEY_free(m_pubKey);
    }
    if(m_priKey)
    {
        EVP_PKEY_free(m_priKey);
    }
}

void RsaCrypto::parseStringToKey(QByteArray data, KeyType type)
{
    BIO *bio = BIO_new_mem_buf(data.data(), data.size());
    assert(bio != NULL);

    if(type == PublicKey)
    {
        PEM_read_bio_PUBKEY(bio, &m_pubKey, NULL, NULL);
    }
    else
    {
        PEM_read_bio_PrivateKey(bio, &m_priKey, NULL, NULL);
    }
    BIO_free(bio);
}

void RsaCrypto::generateRsaKey(KeyLength bits, QByteArray pub, QByteArray pri)
{
    //创建密钥上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    //上下文初始化
    int ret = EVP_PKEY_keygen_init(ctx);
    assert(ret == 1);
    //指定密钥对长度
    ret = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits);
    assert(ret == 1);

    //生成密钥对
    ret = EVP_PKEY_generate(ctx, &m_priKey);
    assert(ret == 1);
    //释放上下文
    EVP_PKEY_CTX_free(ctx);

    //将私钥写到磁盘文件中
    BIO* bio = BIO_new_file(pri.data(), "wb");
    ret = PEM_write_bio_PrivateKey(bio, m_priKey,
                                       NULL, NULL,
                                       0, NULL, NULL);
    assert(ret == 1);
    BIO_flush(bio);
    BIO_free(bio);

    //将公钥钥写到磁盘文件中
    bio = BIO_new_file(pub.data(), "wb");
    ret = PEM_write_bio_PUBKEY(bio, m_priKey);
    assert(ret == 1);
    BIO_flush(bio);
    BIO_free(bio);
}

QByteArray RsaCrypto::pubKeyEncrypt(QByteArray data)
{
    //创建密钥上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_pubKey, NULL);
    assert(ctx != NULL);
    //设置加密
    int ret = EVP_PKEY_encrypt_init(ctx);
    assert(ret == 1);

    //设置 RSA 非对称加密填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret == 1);

    //使用公钥加密

    size_t outLen = 0;
    ret = EVP_PKEY_encrypt(ctx,
                         NULL, &outLen,
                           (const unsigned char *)data.data(), data.size());
    assert(ret == 1);
    unsigned char *out = new unsigned char[outLen];
    ret = EVP_PKEY_encrypt(ctx,
                           out, &outLen,
                           (const unsigned char *)data.data(), data.size());
    assert(ret == 1);

    Base64 base;

    QByteArray retStr = base.encode((char*)out, outLen);
    //释放资源
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return retStr;
}

QByteArray RsaCrypto::priKeyDecrypt(QByteArray data)
{
    //创建密钥上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_priKey, NULL);
    assert(ctx != NULL);
    //设置解密
    int ret = EVP_PKEY_decrypt_init(ctx);
    assert(ret == 1);

    //设置 RSA 非对称加密填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);
    assert(ret == 1);

    Base64 base;
    data = base.decode(data);
    //使用私钥解密
    size_t outLen = 0;
    ret = EVP_PKEY_decrypt(ctx,
                           NULL, &outLen,
                           (const unsigned char *)data.data(), data.size());
    assert(ret == 1);
    unsigned char *out = new unsigned char[outLen];
    ret = EVP_PKEY_decrypt(ctx,
                           out, &outLen,
                           (const unsigned char *)data.data(), data.size());
    assert(ret == 1);

    QByteArray retStr((char*)out, outLen);
    //释放资源
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return retStr;
}

QByteArray RsaCrypto::sign(QByteArray data, QCryptographicHash::Algorithm hash)
{
    //计算哈希值
    QCryptographicHash h(hash);
    h.addData(data);
    QByteArray md = h.result();


    //创建密钥上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_priKey, NULL);
    assert(ctx != NULL);
    //设置解密
    int ret = EVP_PKEY_sign_init(ctx);
    assert(ret == 1);

    //设置 RSA 非对称加密填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);


    //设置签名使用的哈希算法
    ret = EVP_PKEY_CTX_set_signature_md(ctx, m_hashMethod.value(hash)());
    assert(ret == 1);

    //使用私钥解密
    size_t outLen = 0;
    ret = EVP_PKEY_sign(ctx,
                           NULL, &outLen,
                           (const unsigned char *)md.data(), md.size());
    assert(ret == 1);
    unsigned char *out = new unsigned char[outLen];
    ret = EVP_PKEY_sign(ctx,
                           out, &outLen,
                           (const unsigned char *)md.data(), md.size());
    assert(ret == 1);

    Base64 base;
    QByteArray retStr = base.encode((char*)out, outLen);
    //释放资源
    delete[]out;
    EVP_PKEY_CTX_free(ctx);
    return retStr;
}

bool RsaCrypto::verify(QByteArray sign, QByteArray data, QCryptographicHash::Algorithm hash)
{
    Base64 base;
    sign = base.decode(sign);
    //计算哈希值
    QCryptographicHash h(hash);
    h.addData(data);
    QByteArray md = h.result();


    //创建密钥上下文
    EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(m_pubKey, NULL);
    assert(ctx != NULL);
    //设置解密
    int ret = EVP_PKEY_verify_init(ctx);
    assert(ret == 1);

    //设置 RSA 非对称加密填充方式
    ret = EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
    assert(ret == 1);


    //设置签名使用的哈希算法
    ret = EVP_PKEY_CTX_set_signature_md(ctx, m_hashMethod.value(hash)());
    assert(ret == 1);

    //签名校验
    ret = EVP_PKEY_verify(ctx,
                        (const unsigned char *)sign.data(), sign.size(),
                        (const unsigned char *)md.data(), md.size());


    EVP_PKEY_CTX_free(ctx);

    if(ret == 1)
    {
        return true;
    }

    return false;
}
