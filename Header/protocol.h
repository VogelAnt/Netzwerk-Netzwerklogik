#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <vector>
#include <QByteArray>
#include <QString>


/**
 * @brief The CMD enum
 * Declaration of the possible commands with uint8_t values
 */
enum CMD : uint8_t
{
    Aushandeln_Spielfeldparamter = 0x01,
    Anforderung_Spielbeginn = 0x02,
    Schuss = 0x03,
    Antwort_Anfrage = 0x10,
    Antwort_Schuss = 0x11,
    optionale_Kommandos = 0x81,
    message = 0x82
};


/**
 * @brief The GameParameter struct
 *Contains the payload for game parameters
 */
struct GameParameter
{
    /**
     *Default Constructor
      **/
    GameParameter() = default;
    /**
     * @brief GameParameter
     * explicitely initializes data as a uint8 vector
     * @param[in] data called by value
     */
    explicit GameParameter(std::vector<uint8_t> &data);
    /**
     * @brief toData
     * @return returns a uint8 vector
     */
    std::vector<uint8_t> toData() const;
    /**
     * Declared parameters
     */
    uint8_t FeldgroesseXRichtung;
    uint8_t FeldgroesseYRichtung;
    uint8_t Anzahl_Schlachtschiffe;
    uint8_t Anzahl_Kreuzer;
    uint8_t Anzahl_Zerstoerer;
    uint8_t Anzahl_Uboote;
};

/**
 * @brief The Shot struct
 * Contains payload for shot coordinates
 */
struct Shot
{
    Shot() = default;
    explicit Shot(std::vector<uint8_t> &data);
    /**
     * @brief toData
     * constant member function
     * @return returns a uint8_t std::vector
     */
    std::vector<uint8_t> toData() const;

    uint8_t x;
    uint8_t y;
};

/**
 * @brief The value_Antwort_Anfrage enum
 * Contains the unint8_t values for status codes of game request replies
 */
enum value_Antwort_Anfrage : uint8_t
{
    Ok = 0x00,
    abgelehnt_oder_nicht_bereit = 0x01,
    nicht_unterstuetzt = 0x02,
    Nachricht_unvollstaendig = 0x03,
    ACK_time_out = 0x04,
    value_Antwort_Anfrage_sonstiger_Fehler = 0x20
};

/**
 * @brief The AnswerRequest struct
 * contains payload for AnswerRequest
 */
struct AnswerRequest
{
    AnswerRequest() = default;
    AnswerRequest(std::vector<uint8_t> &data);
    /**
     * @brief toData
     * constant member function
     * @return returns a unint8_t std::vector
     */
    std::vector<uint8_t> toData() const;

    /**
     * @brief status
     * status code for reply ro game request
     */
    value_Antwort_Anfrage status;
};

/**
 * @brief The value_Antwort_Schuss enum
 * Contains the unint8_t values for status codes of game request replies
 */
enum value_Antwort_Schuss : uint8_t
{
    nicht_getroffen = 0x00,
    getroffen = 0x01,
    getroffen_und_versenkt = 0x02,
    getroffen_und_versenkt_ENDE = 0x03,
    nicht_an_der_Reihe = 0x10,
    ausserhalb_des_Feldes = 0x11,
    value_Antwort_Schuss_sonstiger_Fehler = 0x20
};

/**
 * @brief The AnswerShot struct
 *
 */
struct AnswerShot
{
    /**
      *default constructor
      **/
    AnswerShot() = default;
    AnswerShot(std::vector<uint8_t> &data);
    /**
     * @brief toData
     * constant member function
     * @return
     * returns a unint8 vector
     */
    std::vector<uint8_t> toData() const;
    /**
     * @brief status
     * status code for response to shot
     */
    value_Antwort_Schuss status;
    /**
     * @brief sankedShips
     * pair is a container that will hold the x and y coordinates of a ship that has been sanked
     */
    std::vector<std::pair<uint8_t, uint8_t>> sankedShips;
};

/**
 * @brief The Message struct
 * Initialises an object with data vector
 */
struct Message
{
    Message(QString& str);
    Message(std::vector<uint8_t> &data);
    /**
     * @brief toData
     * constant memberfunction toData that generates the payload out of an object for transfer via TCP connection
     * @return
     */
    std::vector<uint8_t> toData() const;

    QString str;
};



#endif // PROTOCOL_H
