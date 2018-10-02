#ifndef SERVER_H
#define SERVER_H

#include <memory>

#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QObject>
#include <QSslSocket>
#include <QByteArray>
#include <QDataStream>
#include <QString>
#include "protocol.h"

/**
 * @brief The Connection class
 * Contains methods and attributes that enable communication with Logic, GUI and reading/writing from/to a socket
 * You could say this is my "network" class
 */


class Connection : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Connection Default Constructor
     * @param parent points to 0 because why not
     */
    Connection(QObject *parent = 0);

public:
    //public methods are for the information flow of logic-->network
    /**
     * @brief sendGameStart
     *Enables logic to initiate a game
     */
    void sendGameStart();

    /**
     * @brief sendShot
     * Enables logic to send shot coordinates
     * @param[in] shot contains coordinate tupel
     */
    void sendShot(const Shot& shot);

    /**
     * @brief sendAnswerRequest
     * Enables logic to reply to a game request
     * @param[in] answer contains status code and payload
     */
    void sendAnswerRequest(const AnswerRequest& answer);

    /**
     * @brief sendAnswerShot
     * Enables logic to send an answer to shot
     * @param[in] answer contains status code and payload
     */

    void sendAnswerShot(const AnswerRequest& answer);

    /**
     * @brief sendMessage2
     *superfluous chat implementation i couldn't finish because I decided to use a dumber more difficult one -see ChatServer/ChatClient
     * If it ain't overly complicated what's the point of using C++ anyway
     *
     */
    void sendMessage2(const Message& msg);

    /**
     * @brief sendOptional
     * Not in use, still cool to have
     */
    void sendOptional();


    //the following signals are necessary for the information flow network-->logic/GUI
signals:

    /**
     * @brief connected
     * necessary to inform GUI if a connection has been established
     */
    void connected();

    /**
     * @brief receiveGameParams
     *method used to extract the game parameters from an incoming data stream
     * @param[in] params
     */
    void receiveGameParams(const GameParameter& params);

    /**
     * @brief receiveGameStart
     * method that tells logic somebody requested a new game
     */
    void receiveGameStart();

    /**
     * @brief
     * for extracting shot coordinates
     * @param[in] shot the shot coordinates
     */
    void receiveShot(const Shot& shot);
    /**
     * @brief
     * for extracting the answer to our request
     * @param[in] answer value to our request
     */
    void receiveAnswerRequest(const AnswerRequest& answer);

    /**
     * @brief
     * contains statuscode and coordinates
     * @param answer
     * @param dummy
     * @param dummy2
     */
    void receiveAnswerShot(const AnswerShot& answer, int dummy, int dummy2);

    /**
     * @brief old chat function
     * @param msg
     */
    void receiveMessage(const Message& msg);

    /**
     * @brief
     * needed for receive optional requests, you could (if you were smarter than me) implement a chat function for example
     */
    void receiveOptional();

public slots:

    /**
     * @brief
     * extracts information from socket and determines its function
     */
    void processMessage();

    /**
     * @brief
     * structures and reads information from the socket
     * @param cmd command
     * @param data payload
     */

    /**
     * @brief
     *
     * @param cmd
     * @param data
     */
    void sendMessage(CMD cmd, const std::vector<uint8_t>& data);

    /**
     * @brief
     * kills all ongoing connections by closing all sockets
     */
    void disconnect();


protected:
    /**
     * a TCP socket used for all information flows except for chat
     */
    QTcpSocket *socket;
    //QTcpSocket *chatsocket;
    bool _isConnected;

};



//Server_Klasse
/**
 * @brief The Server class
 * A TCP server whose default port is 1200
 */
class Server : public Connection
{
    Q_OBJECT

public:
    explicit Server(quint16 port=1200, QObject *parent = nullptr);

    /**
     * @brief
     * will send a connected client the game parameters decided by the player
     * @param params
     */
    void sendGameParams(const GameParameter& params);

public slots:
    /**
      * @brief
      * establishes connection, informs GUI about said connection
      */
     void onConnect();

private:
     /**
      * a QTCP server object
      */
    QTcpServer *_server;

};



/**
 * @brief The Client class
 *A Client that can connect to a desired port and host address
 */
class Client : public Connection
{
    Q_OBJECT

public:
    explicit Client(QHostAddress addr, quint16 port, QObject *parent=nullptr);

public slots:
    /**
      *@brief
    * establishes connection, informs GUI about said connection
    */
     void onConnect();

private:
};

#endif // SERVER_H
