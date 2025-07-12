#include "loading.h"
#include "card.h"
#include "login.h"
#include "aescrypto.h"
#include "rsacrypto.h"

#include <QApplication>
#include <QDebug>
#include <QFile>

void test()
{
    AesCrypto aes(AesCrypto::Algorithm::AES_CBC_128, "1234567887654321");
    QByteArray text = aes.enctypt("cwcwqecqdc23r24t你好你好");

    qDebug() << text.data() << Qt::endl;

    text = aes.dectypt(text);
    qDebug() << text.data() << Qt::endl;

    RsaCrypto rsa;
    rsa.generateRsaKey(RsaCrypto::Bits_2K);

    RsaCrypto rsa1("public.pem", RsaCrypto::PublicKey);
    QByteArray tmp = "415478214617cgshccsavf你好你好";
    tmp = rsa1.pubKeyEncrypt(tmp);
    qDebug() << tmp.data() << Qt::endl;


    RsaCrypto rsa2("private.pem", RsaCrypto::PrivateKey);
    tmp = rsa2.priKeyDecrypt(tmp);
    qDebug() << tmp.data() << Qt::endl;

    tmp = rsa2.sign(text);
    bool flag = rsa1.verify(tmp, text);
    qDebug() << flag << Qt::endl;

}


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");

    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(true);

    //加载资源文件
    QFile file(":/conf/style.qss");
    file.open(QFile::ReadOnly);
    QByteArray all = file.readAll();
    a.setStyleSheet(all);
    file.close();

    Loading* loadingScreen = new Loading();

    QObject::connect(loadingScreen, &Loading::loadingFinished, loadingScreen, [&]()
    {
        loadingScreen->deleteLater();

        Login* loginDialog = new Login();
        int ret = loginDialog->exec();

        if (ret != QDialog::Accepted)
        {
            loginDialog->deleteLater();
            QApplication::quit();
        }
        else
        {
            loginDialog->deleteLater();
        }
    });

    loadingScreen->show();

    return a.exec();
}
