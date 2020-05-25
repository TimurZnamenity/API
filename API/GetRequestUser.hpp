#ifndef PROJECT_GET_REQUEST_USER_HPP
#define PROJECT_GET_REQUEST_USER_HPP

#include "Global.hpp"
#include "GetRequest.hpp"
#include "User.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Запрос "Данные пользователя".
 */
class GetRequestUser : public GetRequest
{
    Q_OBJECT

public:
    GetRequestUser(QObject* parent = nullptr);
    ~GetRequestUser() {
        PROJECT_QDEBUG << "Request <User> destroyed";
    }

private:

    /**
     * Обрабатывает ответ на запрос.
     */
    void parseResponse(const QJsonObject& jsonObject);

    /**
     * Обрабатывает ошибки в ответе на запрос.
     */
//    void parseError(const QJsonObject& jsonObject);

    /**
     * Формирует строку запроса api.
     * @return API-строка
     */
    QString constructApiString() const;

signals:
    /**
     * Сигнал сообщает что данные получены и изменены.
     */
    void userUpdated( QSharedPointer<User> userPtr );
};

PROJECT_NAMESPACE_END

#endif // PROJECT_GET_REQUEST_USER_HPP
