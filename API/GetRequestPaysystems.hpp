#ifndef PROJECT_GET_REQUEST_PAYSYSTEMS_HPP
#define PROJECT_GET_REQUEST_PAYSYSTEMS_HPP

#include "Global.hpp"
#include "GetRequest.hpp"
#include "Paysystems.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Запрос "Платежные системы".
 */
class GetRequestPaysystems : public GetRequest
{
    Q_OBJECT

public:
    GetRequestPaysystems(QObject* parent = nullptr);
    ~GetRequestPaysystems() {
        PROJECT_QDEBUG << "Request <Paysystems> destroyed";
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
    void paysystemsUpdated( QSharedPointer<Paysystems> userPtr );
};

PROJECT_NAMESPACE_END

#endif // PROJECT_GET_REQUEST_PAYSYSTEMS_HPP
