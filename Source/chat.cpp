#include "chat.h"

#include <QDebug>

void Chat::processChatMessage()
{
    qDebug() << "in Chat::processChatMessage";
    QString nachricht;
    _chatstream >> nachricht;
    qDebug() << "\treceived message " << nachricht;
    emit chatReceiveMessage(nachricht);

}

void Chat::sendChatMessage(QString nachricht)
{
    qDebug() << "in Chat::sendChatMessage";
    qDebug() << "\tsending message" << nachricht;
   _chatstream << nachricht;
}


/* -------------------------- Server -------------------------------------- */


ChatServer::ChatServer()
    :  _chatServer(new QTcpServer(this))
{
    connect(_chatServer, &QTcpServer::newConnection, this, &ChatServer::onConnect);
    _chatServer->listen(QHostAddress::Any, 8000);

    qDebug() << "ChatServer:ChatServer()";
}

void ChatServer::onConnect()
{
    qDebug() << "ChatServer::onConnect";
    _chatsocket = _chatServer->nextPendingConnection();
    //close chatsocket
    _chatServer->close();
    _chatstream.setDevice(_chatsocket);

    //GUI kann reagieren
    connect(_chatsocket, &QTcpSocket::readyRead, this, &ChatServer::processChatMessage);
}



/* -------------------------- Client -------------------------------------- */

ChatClient::ChatClient(QHostAddress addr)
{
    _chatsocket = new QTcpSocket(this);
    _chatstream.setDevice(_chatsocket);

    connect(_chatsocket, SIGNAL(connected()), this, SLOT(onConnect()));
    _chatsocket->connectToHost(addr, 8000);
    qDebug() << "ChatClient:ChatClient()";
}

void ChatClient::onConnect()
{
    qDebug() << "ChatClient::onConnect";
   connect(_chatsocket, &QTcpSocket::readyRead, this, &ChatClient::processChatMessage);
}
