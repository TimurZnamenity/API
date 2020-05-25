#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "Global.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Хранит и парсит токен
 */
class Token
{
/* ############################################################################################## */
public:
/* ############################################################################################## */

    Token();

    /**
     * Возвращает токен
     */
    static QString     token();

    /**
     * Извлекает из токена сериализованную строку Payload
     */
    static QString     extractRawPayloadString(const QString& token);

    /**
     * Десериализирует строку Payload
     */
    static QString     deserializePayloadString(const QString& rawPayloadString);

    /**
     * Преобразует строку Payload в JSON-объект
     */
    static QJsonObject convertPayloadString(const QString& payloadString);

    /**
     * Извлекает ИД пользователя из JSON-объекта Payload
     */
    static QString     extractUserIdFromPayload(const QJsonObject& payload);

    /**
     * Извлекает ИД пользователя из токена за одно действие
     */
    static QString     extractUserIdFromToken();
};

PROJECT_NAMESPACE_END

#endif // TOKEN_HPP
