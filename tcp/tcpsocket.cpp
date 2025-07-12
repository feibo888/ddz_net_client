#include "tcpsocket.h"
#include "qdebug.h"


TcpSocket::TcpSocket(QObject *parent)
{
#ifdef Q_OS_WIN
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif
}

TcpSocket::TcpSocket(QByteArray ip, unsigned short port, QObject *parent) : TcpSocket(parent)
{
    connectToHost(ip, port);
}

TcpSocket::~TcpSocket()
{
#ifdef Q_OS_WIN
    WSACleanup();
#endif
}

bool TcpSocket::connectToHost(QByteArray ip, unsigned short port)
{
    assert(port > 0 && port <= 65535);
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    assert(m_socket > 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.data());
    int ret = ::connect(m_socket, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));

    bool flag = ret == 0 ? true : false;
    return flag;

}

QByteArray TcpSocket::recvMsg(int timeout)
{
    bool flag = readTimeout(timeout);
    QByteArray array;
    if(flag)
    {
        //发送数据 = 数据头（headLen）+ 数据块
        int headLen = 0;
        int ret = readn((char*)&headLen, sizeof(int));
        if(ret == 0)
        {
            return QByteArray();
        }
        headLen = ntohl(headLen);

        char* data = new char[headLen];
        assert(data);

        ret = readn(data, headLen);
        if(ret == headLen)
        {
            array = QByteArray(data, headLen);
        }
        else
        {
            array = QByteArray();
        }
        delete[]data;
    }
    return array;
}

bool TcpSocket::sendMsg(QByteArray msg, int timeout)
{
    bool flag = writeTimeout(timeout);
    if(flag)
    {
        //发送数据 = 数据头（headLen）+ 数据块

        //qDebug() << "数据头长度： (小端)" << msg.size() << Qt::endl;
        int headLen = htonl(msg.size());
        //qDebug() << "数据头长度： (大端)" << headLen << Qt::endl;

        int length = sizeof(int) + msg.size();
        char* data = new char[length];
        memcpy(data, &headLen, sizeof(int));
        memcpy(data + sizeof(int), msg.data(), msg.size());

        int ret = writen(data, length);
        flag = ret == length ? true : false;

        delete[]data;
    }
    return flag;
}

void TcpSocket::disConnect()
{
    if(m_socket)
    {
#ifdef Q_OS_WIN
        closesocket(m_socket);
#endif
#ifdef Q_OS_LINUX
        close(m_socket);
#endif
    }
}

bool TcpSocket::readTimeout(int timeout)
{
    if(timeout == -1)
    {
        return true;
    }

    //检测读缓冲区

#ifdef Q_OS_WIN
    int nfds = 0;
#endif
#ifdef Q_OS_LINUX
    int nfds = m_socket + 1;
#endif

    fd_set rdset;
    FD_ZERO(&rdset);
    FD_SET(m_socket, &rdset);

    struct timeval tmout;
    tmout.tv_sec = timeout;
    tmout.tv_usec = 0;

    int ret = select(nfds, &rdset, NULL, NULL, &tmout);
    bool flag = ret > 0 ? true : false;
    return flag;
}

bool TcpSocket::writeTimeout(int timeout)
{
    if(timeout == -1)
    {
        return true;
    }

    //检测写缓冲区

#ifdef Q_OS_WIN
    int nfds = 0;
#endif
#ifdef Q_OS_LINUX
    int nfds = m_socket + 1;
#endif

    fd_set wset;
    FD_ZERO(&wset);
    FD_SET(m_socket, &wset);

    struct timeval tmout;
    tmout.tv_sec = timeout;
    tmout.tv_usec = 0;

    int ret = select(nfds, NULL, &wset, NULL, &tmout);
    bool flag = ret > 0 ? true : false;
    return flag;
}

int TcpSocket::readn(char *buf, int count)
{
    int last = count;   //剩余的字节数
    int size = 0;       //每次读的字节数
    char* pt = buf;
    while(last > 0)
    {
        if((size = recv(m_socket, pt, last, 0)) < 0)
        {
            perror("recv");
        }
        else if(size == 0)  //返回 0 时，表示对端已关闭连接，所以break
        {
            break;
        }
        pt += size;
        last -= size;
    }
    return count - last;
}

int TcpSocket::writen(const char *buf, int count)
{
    int last = count;
    int size = 0;       //每次写的字节数
    const char* pt = buf;
    while(last > 0)
    {
        if((size = send(m_socket, pt, last, 0)) < 0)
        {
            perror("send");
        }
        else if(size == 0)  //返回 0 通常表示没有数据被发送出去（可能是由于暂时的网络阻塞或缓冲区已满）,所以continue
        {
            continue;
        }
        pt += size;
        last -= size;
    }
    return count - last;
}
