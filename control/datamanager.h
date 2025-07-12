#ifndef DATAMANAGER_H
#define DATAMANAGER_H


#include <Cards.h>
#include <QByteArray>
#include "communication.h"

class Communication;

//饿汉模式
class DataManager
{
public:
    enum GameMode{Single, NetWork};
    enum RoomMode{Auto, Manual};
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    static DataManager* getInstance();

    //设置数据
    void setUserName(QByteArray name);
    void setIP(QByteArray IP);
    void setPort(QByteArray port);
    void setCommunication(Communication* comm);
    void setRoomName(QByteArray roomName);
    void setCards(Cards cards, Cards last);
    void setGameMode(GameMode mode);
    void setRoomMode(RoomMode mode);


    //获取数据
    QByteArray getUserName();
    QByteArray getIP();
    QByteArray getPort();
    Communication* getCommunication();
    QByteArray getRoomName();
    Cards getCards();
    Cards getLastCards();
    bool isNetworkMode();
    bool isManualRoom();

private:
    DataManager() = default;
    static DataManager* m_data;

    QByteArray m_userName;
    QByteArray m_roomName;
    QByteArray m_ip = "172.26.168.246";       //47.121.134.236
    QByteArray m_port = "10000";
    Communication* m_comm;
    Cards m_cards;
    Cards m_last;
    GameMode m_mode;
    RoomMode m_roomMode;
};

#endif // DATAMANAGER_H
