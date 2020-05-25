#include "Token.hpp"
#include "AuthUtils.hpp"


using namespace PROJECT;



/* ---------------------------------------------------------------------------------------------- */
Token::Token()
{

}


/* ---------------------------------------------------------------------------------------------- */
QString Token::token()
{
    return AuthUtils::getAccessTokenData();
}


/* ---------------------------------------------------------------------------------------------- */
QString Token::extractRawPayloadString(const QString& token)
{
    // части токена разделяются точкой (header.payload.signature)
    QStringList list = token.split(".");
    if( list.count() < 2 )
    {
        qDebug() << "Token: Wrong token format";
        return "";
    }

    return list.at(1);
}


/* ---------------------------------------------------------------------------------------------- */
QString Token::deserializePayloadString(const QString& rawPayloadString)
{
    QByteArray text = QByteArray::fromBase64( rawPayloadString.toLocal8Bit(),
                                              QByteArray::Base64UrlEncoding );

    return QString(text);
}


/* ---------------------------------------------------------------------------------------------- */
QJsonObject Token::convertPayloadString(const QString& payloadString)
{
    QJsonDocument   jsonDoc;
    QJsonParseError parseError;

    jsonDoc = QJsonDocument::fromJson(payloadString.toLocal8Bit(), &parseError);

    if(parseError.error != QJsonParseError::NoError)
    {
        qDebug()   << payloadString;
        qWarning() << "Token: Json parse error: " << parseError.errorString();
        return QJsonObject();
    }

    if( !jsonDoc.isObject() )
    {
        qDebug() << "Token: Not a QJsonObject";
        return QJsonObject();
    }

    return jsonDoc.object();
}

/* ---------------------------------------------------------------------------------------------- */
QString Token::extractUserIdFromPayload(const QJsonObject& payload)
{
    // в payload берем поле sub
    QJsonValue sub;
    QString idString;

    // берем JSONValue
    sub = payload["sub"];
    if( sub.isUndefined() )
    {
        qWarning() << "Token: field \"sub\" is undefined";
        return "";
    }

    // берем строку
    idString = sub.toString();
    if( idString.isNull() )
    {
        qWarning() << "Token: field \"sub\" is not a string";
        return "";
    }

    return idString;
}


/* ---------------------------------------------------------------------------------------------- */
QString Token::extractUserIdFromToken()
{
    // токен
    QString token = Token::token();
    // разбиваем токен на части и выделяем payload
    QString rawPayloadString = Token::extractRawPayloadString( token );
    // десериализируем payload
    QString payloadString = Token::deserializePayloadString( rawPayloadString );
    // выделяем payload
    QJsonObject payload = Token::convertPayloadString( payloadString );
    // извлкаем из payload ИД пользователя
    QString idString = Token::extractUserIdFromPayload(payload);

    return  idString;
}
