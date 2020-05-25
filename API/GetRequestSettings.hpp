#ifndef PROJECT_GET_REQUEST_SETTINGS_HPP
#define PROJECT_GET_REQUEST_SETTINGS_HPP

#include "Global.hpp"
#include "GetRequest.hpp"
#include "Settings.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Запрос "Настройки".
 */
class GetRequestSettings : public GetRequest
{
    Q_OBJECT

public:
    GetRequestSettings(QObject* parent = nullptr);
    ~GetRequestSettings() {
        PROJECT_QDEBUG << "Request <Settings> destroyed";
    }

private:

    /**
     * Обрабатывает ответ на запрос.
     */
    void parseResponse(const QJsonObject& jsonObject) override;

    /**
     * Обрабатывает ошибки в ответе на запрос.
     */
//    void parseError(const QJsonObject& jsonObject) override;

    /**
     * Формирует строку запроса api.
     */
    QString constructApiString() const override;

signals:
    /**
     * Сигнал сообщает что данные получены и изменены.
     */
    void settingsUpdated( QSharedPointer<Settings> userPtr );
};

PROJECT_NAMESPACE_END

#endif // PROJECT_GET_REQUEST_SETTINGS_HPP
