#include "GetRequestTransactions.hpp"



using namespace PROJECT;


/* ############################################################################################## */
/* Конструкторы / деструктор */
/* ############################################################################################## */

GetRequestTransactions::GetRequestTransactions(QObject* parent) : GetRequest(parent)
{
    m_statuses = nullptr;
    m_types    = nullptr;
}

/* ---------------------------------------------------------------------------------------------- */
GetRequestTransactions::GetRequestTransactions(QVector<TransactionStatus>* statuses,
                                               QObject* parent)
    : GetRequest(parent)
{
    m_statuses = statuses;
    m_types    = nullptr;
}

/* ---------------------------------------------------------------------------------------------- */
GetRequestTransactions::GetRequestTransactions(QVector<TransactionType>* types,
                                               QObject* parent)
    : GetRequest(parent)
{
    m_statuses = nullptr;
    m_types    = types;
}

/* ---------------------------------------------------------------------------------------------- */
GetRequestTransactions::GetRequestTransactions(QVector<TransactionStatus>* statuses,
                                               QVector<TransactionType>*   types,
                                               QObject* parent)
    : GetRequest(parent)
{
    m_statuses = statuses;
    m_types    = types;
}


/* ############################################################################################## */
/* Методы */
/* ############################################################################################## */

//void GetRequestTransactions::get()
//{

//}

/* ---------------------------------------------------------------------------------------------- */
void GetRequestTransactions::parseResponse(const QJsonObject& jsonObject)
{
    QJsonObject   timeObject;
    QJsonObject   dataObject;
    QJsonArray    dataArray;

    QSharedPointer<Transactions> m_transactions{new Transactions()};

    if( jsonObject["data"].isArray() )
        dataArray = jsonObject["data"].toArray();
    else
    {
        PROJECT_QDEBUG << "Transactions does not contain JsonArray";
        return; // обработка ошибки
    }

    // пробегаем по всем транзакциям
    for( int i = 0; !dataArray.at(i).isUndefined(); i++ )
    {
        // проверяем
        if( dataArray.at( i ).isObject() )
            dataObject = dataArray.at( i ).toObject();
        else
        {
            PROJECT_QDEBUG << "Transaction is not JsonObject";
            return;
        }

        // выгребаем транзакцию
        Transaction transaction;
        transaction.id         = dataObject["id"].toInt();
        transaction.name       = dataObject["what"].toString();
        transaction.sum        = Money( dataObject["sum"].toString() );
        transaction.commission = Money( dataObject["commission"].toString() );
        transaction.currency   = dataObject["currency"].toString();
        transaction.status     = Transaction::toStatus( dataObject["status"].toString() );
        transaction.type       = Transaction::toType( dataObject["type"].toString() );
        transaction.comment    = dataObject["comment"].toString();

        QString timeStampString
                               = dataObject["created_at"].toString();
        transaction.timeStamp  = QDateTime::fromString( timeStampString,
                                                        "yyyy-MM-dd hh:mm:ss" );

        // добавляем ее в список
        m_transactions->push_back( transaction );
    }

    PROJECT_QDEBUG << "Before emit <Transactions>";

    // сообщаем View, что Model обновилась
    emit transactionsUpdated( m_transactions );

    PROJECT_QDEBUG << "Request <Transactions> parsed";
}

/* ---------------------------------------------------------------------------------------------- */
//void GetRequestTransactions::parseError(const QJsonObject&)
//{
//    return;
//}

/* ---------------------------------------------------------------------------------------------- */
QString GetRequestTransactions::constructApiString() const
{
    QString statusFilters;
    if( m_statuses )
        for (auto status : *m_statuses)
            statusFilters += "&status[]=" + Transaction::fromStatus( status );

    QString typeFilters;
    if( m_types )
        for (auto type : *m_types)
            typeFilters += "&type[]=" + Transaction::fromType( type );

    // извлекаем ИД пользователя из токена
    QString idString = userId();

    // здесь подставляем ИД пользователя в запрос
    QString apiStr = QString("%1%2%3%4%5")
            .arg("/transactions")
            .arg("?user_id=")
            .arg(idString)
            .arg(statusFilters)
            .arg(typeFilters);

    return apiStr;
}
