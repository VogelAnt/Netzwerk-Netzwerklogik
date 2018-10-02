/* Noch zu tun:
 * -Port 80 sollte fix sein
 * -Eingabe von Hostadresse ist wegen NAT doch immer fix? --> 127.0.0.1
 * -Koordination mit GUI sollte demnach eigentlich recht simpel sein, ui->connect, disconnect...
 * -mit Logik muss eine switch Case die Einzelnen Bytes übergeben. d.h. wir machen eine receive Fkt die das ganze mit ner switch Case entschlüsselt
 * -Bei manuellem Disconnect sollte im Chat eine Nachricht "playername disconnected" kommen
 * -Da wir nur auf Port 80 bzw. 81 verbinden/horchen -->ev zuerst 80 checken, dann nach erneutem button->click auf 81 verbinden "no connection on port 80"
 * -Logik schicken wer als erstes Anfrage zu Spielstart geschickt hat
 *
 * GUI -> NW
 * Client: connect(von ButtonConnect Adresse kriegen)
 * Server: connect(start Server, listen on port 80... keine Parameter)
 * Disconnect Button : disconnect(keine Parameter, schließt Socket)
 * Chat: sendMessage() Button (zuerst einmal nur Nachricht schicken oder empfangen)
 *
 *
 * NW -> GUI
 * connection successful schicken
 * disconnected Bestätigung sobald tatsächlich disconnected
 * Chat Nachricht von Gegner soll weitergeleitet werden
 *
 *
 * NW -> Logik
 * Signal ob Player 1 oder Player 2 gestartet hat, emit signal(int player), 1 sind wir, 2 der Andere
 * receive
 *
 *
 * Logik -> NW
 * Bytefolge , muss nur noch verpackt und geschickt werden
 *
 * */

#include "communication.h"
#include "protocol.h"
#include <QDebug>

using namespace std;


Connection::Connection(QObject *parent)
    : QObject(parent), socket(nullptr)
{
}

void Connection::disconnect()
{
     //socket->disconnectFromHost();
     socket->close();
     qDebug () << "You are Disconnected";
}

void Server::sendGameParams(const GameParameter &params)
{
    sendMessage(Aushandeln_Spielfeldparamter, params.toData());
}

void Connection::sendGameStart()
{
    sendMessage(Anforderung_Spielbeginn, vector<uint8_t>());
}

void Connection::sendShot(const Shot &shot)
{
    sendMessage(Schuss, shot.toData());
}

void Connection::sendAnswerRequest(const AnswerRequest &answer)
{
    sendMessage(Antwort_Anfrage, answer.toData());
}

void Connection::sendAnswerShot(const AnswerRequest &answer)
{
    sendMessage(Antwort_Schuss, answer.toData());
}
//superfluous
void Connection::sendMessage2(const Message &msg)
{
    sendMessage(message, msg.toData());
}

//Diese Funktion liest aus der Socket aus
void Connection::processMessage()
{
    qDebug()<<"now in processmessage";
    const size_t Command = 0;
    const size_t PayloadLength = 1;

    /*
     * |cmd|payloadlength|
     *
     *
     *
     *
     * */
    char header[2];
    socket->read(header, 2);
    vector<uint8_t> data;
   /* if(data.size()!= header[PayloadLength]) {
        // emit ungültig to other network
    } else{ */
    for(int i = 0; i < header[PayloadLength]; ++i)
    {
        /*
         * |cmd|payloadlength|byte_1|.....|byte_n|
         *
         *
         *
         *
         * */
        char byte;
        socket->read(&byte, 1);
        //caste damit Datentypen übereinstimmen
        data.emplace_back(static_cast<uint8_t>(byte));
    }
         if(data.size()!= (uint8_t)(header[PayloadLength])) {
         CMD cmd = Antwort_Anfrage;
         std::vector<uint8_t> databack = {abgelehnt_oder_nicht_bereit};
         sendMessage(cmd,databack);

     } else{
    //lese CMD aus header, ermittle um welchen Befehl es sich handelt
    switch(static_cast<CMD>(header[Command]))
    {
    case Aushandeln_Spielfeldparamter:

        emit receiveGameParams(GameParameter(data)); // später
        break;
    case Anforderung_Spielbeginn:
        qDebug()<<"now in anforderung ";
        emit receiveGameStart();
        break;
    case Schuss:
        emit receiveShot(Shot(data));
        break;
    case Antwort_Anfrage:
        emit receiveAnswerRequest(AnswerRequest(data));
        break;
    case Antwort_Schuss:
        emit receiveAnswerShot(AnswerShot(data),0,0);
        break;
    case message:
        emit receiveMessage(Message(data));
        break;
    default:
             // throw std::invalid_argument("unknown command");//ev ändern
        CMD cmd = Antwort_Anfrage;
        std::vector<uint8_t> databack = {abgelehnt_oder_nicht_bereit};
        sendMessage(cmd,databack);
        break;

     }
    }
}


void Connection::sendMessage(CMD cmd, const std::vector<uint8_t>& data)
{
    //Initialisiert ein Bytearray ba mit der Größe 2+data.size() und setzt alle Werte auf 0
    QByteArray ba(2 + static_cast<int>(data.size()), 0);

    ba[0] = static_cast<char>(cmd);
    ba[1] = static_cast<char>(data.size());
    /*
     * ba:
     * |cmd|laenge|
     * */
    qDebug()<<cmd;
    /*
     * |cmd|laenge|>insert<|
     * */
    auto insert = ba.begin() + 2;
    //data ist uint8_t Vektor und enthält das von Logik/GUI geschickte payload
    for(uint8_t b : data)
    {
        qDebug() << b;
        //ByteArray verlangt chars, das Protokoll verwendet aber uint8 Datentypen-->Konvertierung nach char
        *insert = static_cast<char>(b);
        ++insert;
        /*ba:
         *|cmd|laenge|data|data|data|data|...|
         *
         * */
    }
    //
    socket->write(ba);

}




Server::Server(quint16 port, QObject *parent)
    : Connection(parent), _server(new QTcpServer(this))
{
    //Server öffnet Port und horcht, sobald eine neue Verbindung existiert wird onConnect ausgeführt
    connect(_server, &QTcpServer::newConnection, this, &Server::onConnect);
    _server->listen(QHostAddress::Any, port);
}


//erstelle Socket, starte Kommunikation
void Server::onConnect()
{
    socket = _server->nextPendingConnection();
    _server->close();

    qDebug () << "You are now connected";
    //GUI kann reagieren
    emit connected();
    connect(socket, &QTcpSocket::readyRead, this, &Server::processMessage);
}



Client::Client(QHostAddress addr, quint16 port, QObject *parent)
    : Connection(parent)
{   qDebug()<<"in client";
    socket = new QTcpSocket(this);
    //Client verbindet sich mit Socket
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));
    socket->connectToHost(addr, port);

}

void Client::onConnect()
{  // qDebug()<<"in onconnect";
    //sagt der GUI, dass wir verbunden sind
    emit connected();
    connect(socket, &QTcpSocket::readyRead, this, &Client::processMessage);
}



