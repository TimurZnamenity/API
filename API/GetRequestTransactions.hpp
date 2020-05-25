#ifndef PROJECT_GET_REQUEST_TRANSACTIONS_HPP
#define PROJECT_GET_REQUEST_TRANSACTIONS_HPP

#include "Global.hpp"
#include "GetRequest.hpp"
#include "Transactions.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Запрос "Транзакции".
 */
class GetRequestTransactions : public GetRequest
{
    Q_OBJECT

public:
    /**
     * Несколько конструкторов, чтобы комбинировать разные фильтры в запросе.
     * (по статусу, по типу)
     */
    GetRequestTransactions(QObject* parent = nullptr);
    GetRequestTransactions(QVector<TransactionStatus>* statuses,
                           QObject* parent = nullptr);
    GetRequestTransactions(QVector<TransactionType>*   types,
                           QObject* parent = nullptr);
    GetRequestTransactions(QVector<TransactionStatus>* statuses,
                           QVector<TransactionType>*   types,
                           QObject* parent = nullptr);
    ~GetRequestTransactions() {
        PROJECT_QDEBUG << "Request <Transactions> destroyed";
    }

    /**
     * Переопределенный метод отправки.
     * Потому что здесь используется многостраничный запрос
     */
//    void get();

private:

    QVector<TransactionStatus>* m_statuses;
    QVector<TransactionType>*   m_types;

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
    void transactionsUpdated( QSharedPointer<Transactions> userPtr );
};

PROJECT_NAMESPACE_END

#endif // PROJECT_GET_REQUEST_TRANSACTIONS_HPP
