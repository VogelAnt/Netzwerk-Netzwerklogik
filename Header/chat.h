#ifndef CHAT_H
#define CHAT_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDataStream>

/**
 * @brief The Chat class
 * This class contains all the members and functions of a fully functioning text chat
 */
class Chat: public QObject {
    Q_OBJECT
public:
    Chat() = default;

public slots:
    /**
     * @brief
     * function that will deliver a string message
     * @param nachricht the string we use as message
     */
    void sendChatMessage(QString nachricht);

    /**
     * @brief
     * structures and reads information from the chat socket
     */
    void processChatMessage();

signals:
    /**
     * @brief
     * Sends GUI information derived from nachricht
     * @param cmsg
     */
    void chatReceiveMessage(QString cmsg);


protected:
    /**
     * chatsocket TCP socket
     */
    QTcpSocket *_chatsocket = nullptr;

    /**
     *  _chatstream DataStream for reading/writing
     */
    QDataStream _chatstream;
};

/**
 * @brief The ChatClient class
 * in Client mode this class and its members are necessary for the chat function
 */
class ChatClient : public Chat
{
    Q_OBJECT
public:

    /**
     * @brief ChatClient
     * Constructor
     * @param addr the opponents address
     */
    ChatClient(QHostAddress addr);

public slots:
    /**
     * @brief
     * establishes connection
     */
    void onConnect();
};




/**
 * @brief The ChatServer class
 * ChatServer, der Standardmäßig auf port 8000 horcht
 */
class ChatServer : public Chat
{
    Q_OBJECT

public:
    ChatServer();

public slots:
    /**
     * @brief
     * establishes connection
     */
    void onConnect();

private:
    /**
     *  _chatServer is a TCP Server
     */
    QTcpServer *_chatServer = nullptr;

};


#endif // CHAT_H
