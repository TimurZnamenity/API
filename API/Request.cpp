#include "Request.hpp"


#include <stdlib.h>
#include "Token.hpp"


using namespace PROJECT;


/* ############################################################################################## */
/* Конструкторы / деструктор */
/* ############################################################################################## */

Request::Request(QObject* parent) : QObject(parent)
{
    // инитим requester
    m_requester = new Requester(this);

    static QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
                                    // static - это чтобы он потом в реквестере не прокис
    QString  host = PROJECT_HOST;

    m_requester->initRequester( host, &sslConfig );
    m_requester->setToken( Token::token() );
}

/* ---------------------------------------------------------------------------------------------- */
Request::~Request()
{
    delete m_requester;
}


/* ############################################################################################## */
/* Методы */
/* ############################################################################################## */

QString Request::userId() const
{
    return Token::extractUserIdFromToken();
}

/* ---------------------------------------------------------------------------------------------- */
void Request::parseError(const QJsonObject &jsonObject)
{
    // здесь отрабатываем ошибки
    if( jsonObject["error"].isUndefined() )
        return;
}

