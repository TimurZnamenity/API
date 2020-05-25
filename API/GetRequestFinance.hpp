#ifndef PROJECT_GET_REQUEST_FINANCE_HPP
#define PROJECT_GET_REQUEST_FINANCE_HPP

#include "Global.hpp"
#include "GetRequest.hpp"
#include "Finance.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Запрос "Финансы".
 */
class GetRequestFinance : public GetRequest
{
    Q_OBJECT

public:
    GetRequestFinance(QObject* parent = nullptr);
    ~GetRequestFinance() {
        PROJECT_QDEBUG << "Request <Finance> destroyed";
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
     * Формирует строку запроса API.
     * @return Строка запроса API
     */
    QString constructApiString() const override;

signals:
    /**
     * Сигнал сообщает что данные получены и изменены.
     */
    void financeUpdated( QSharedPointer<Finance> userPtr );
};

PROJECT_NAMESPACE_END

#endif // PROJECT_GET_REQUEST_FINANCE_HPP
