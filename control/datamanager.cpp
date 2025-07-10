#include "datamanager.h"

DataManager* DataManager::m_data = new DataManager;


DataManager *DataManager::getInstance()
{
    return m_data;
}

void DataManager::setUserName(QByteArray name)
{
    m_userName = name;
}

void DataManager::setIP(QByteArray IP)
{
    m_ip = IP;
}

void DataManager::setPort(QByteArray port)
{
    m_port = port;
}

void DataManager::setCommunication(Communication *comm)
{
    m_comm = comm;
}

void DataManager::setRoomName(QByteArray roomName)
{
    m_roomName = roomName;
}

void DataManager::setCards(Cards cards, Cards last)
{
    m_cards = cards;
    m_last = last;
}

void DataManager::setGameMode(GameMode mode)
{
    m_mode = mode;
}

void DataManager::setRoomMode(RoomMode mode)
{
    m_roomMode = mode;
}

QByteArray DataManager::getUserName()
{
    return m_userName;
}

QByteArray DataManager::getIP()
{
    return m_ip;
}

QByteArray DataManager::getPort()
{
    return m_port;
}

Communication *DataManager::getCommunication()
{
    return m_comm;
}

QByteArray DataManager::getRoomName()
{
    return m_roomName;
}

Cards DataManager::getCards()
{
    return m_cards;
}

Cards DataManager::getLastCards()
{
    return m_last;
}

bool DataManager::isNetworkMode()
{
    return m_mode == GameMode::NetWork ? true : false;
}

bool DataManager::isManualRoom()
{
    return m_roomMode == RoomMode::Manual ? true : false;
}

