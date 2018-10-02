#include "protocol.h"

#include <string>

using namespace std;

/*
 *
 * toData() will generate the necessary payload out of an object for transmission via TCP
 *
 *
 * */
GameParameter::GameParameter(std::vector<uint8_t> &data)
    : FeldgroesseXRichtung(data[0]),
      FeldgroesseYRichtung(data[1]),
      Anzahl_Schlachtschiffe(data[2]),
      Anzahl_Kreuzer(data[3]),
      Anzahl_Zerstoerer(data[4]),
      Anzahl_Uboote(data[5])
{
}

vector<uint8_t> GameParameter::toData() const
{
    return vector<uint8_t>{
                         FeldgroesseXRichtung,
                         FeldgroesseYRichtung,
                         Anzahl_Schlachtschiffe,
                         Anzahl_Kreuzer,
                         Anzahl_Zerstoerer,
                         Anzahl_Uboote
                     };
}



Shot::Shot(std::vector<uint8_t> &data)
    : x(data[0]),
      y(data[1])
{
}

std::vector<uint8_t> Shot::toData() const
{
    return vector<uint8_t>{x, y};
}

AnswerRequest::AnswerRequest(std::vector<uint8_t> &data)
    : status(static_cast<value_Antwort_Anfrage>(data[0]))
{

}

std::vector<uint8_t> AnswerRequest::toData() const
{
    return vector<uint8_t>{status};
}

//initialisiert Status mit value_Antwort_Schuss an Position 0 in data
AnswerShot::AnswerShot(std::vector<uint8_t> &data)
    : status(static_cast<value_Antwort_Schuss>(data[0]))
{
    /*data
     *
     *|status|
     *
     * */


    for(auto it = data.begin() + 1; it != data.end(); it += 2)
    {
        //make pair generiert ein Paar mit dem Typ der Elemente ohne sie zu spezifizieren
        sankedShips.push_back(make_pair(*it, *(it + 1)));
    }
    /*data:
     * status|x1|y1|x2|y3|...|xn|yn|
     *
     * */
}

//Diese Funktion returned die Koordinaten von versenkten Schiffen
std::vector<uint8_t> AnswerShot::toData() const
{
    vector<uint8_t> result;
    result.push_back(status);
    for(auto coordinate : sankedShips)
    {
        result.push_back(coordinate.first);
        result.push_back(coordinate.second);
    }
    return result;
}

Message::Message(QString &str) : str(str)
{
}

Message::Message(std::vector<uint8_t> &data)
{
    string s;
    for(auto b : data)
    {
        s.push_back(b);
    }
    QString::fromUtf8(s.c_str());
}

std::vector<uint8_t> Message::toData() const
{
    vector<uint8_t> result;
    for(auto c : str.toUtf8())
    {
        result.push_back(c);
    }
    return result;
}
