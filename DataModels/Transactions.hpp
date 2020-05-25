#ifndef PROJECT_TRANSACTIONS_HPP
#define PROJECT_TRANSACTIONS_HPP

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QMetaClassInfo>
#include "Money.hpp"
#include "Global.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Статус транзакции.
 */
enum TransactionStatus
{
    /// Успешно.
    STATUS_SUCCESS,
    /// Ожидание.
    STATUS_WAIT,
    /// Отмена.
    STATUS_CANCEL
};

/**
 * Тип транзакции.
 */
enum TransactionType
{
    /// Покупка.
    TYPE_BUY,
    /// Выводы.
    TYPE_CASHOUT,
    /// Донаты.
    TYPE_DONATION
};

/**
 * Транзакция со всеми своими полями.
 */
struct Transaction
{
    /// ИД транзакции.
    int         id;
    /// Имя.
    QString     name;
    /// Сумма транзакции.
    Money       sum;
    /// Валюта.
    QString     currency;
    /// Комиссия.
    Money       commission;
    /// Комментарий.
    QString     comment;
    /// Дата и время транзакции.
    QDateTime   timeStamp;
    /// Статус: Отменен, Успешно, Вручную.
    TransactionStatus
                status;
    /// Тип: donation, cashout.
    TransactionType
                type;

    /**
     * Возвращает комментарий, удалив слово "Комментарий: ".
     * @return чистый комментарий
     */
    QString cleanComment() const {
        QString s = this->comment;
        s.remove("Комментарий: ");
        return s;
    }

    /**
     * Возвращает сумму х множитель в строковом виде.
     * Если множитель = 0, то возвращает только сумму
     * @return сумма х множитель
     */
    QString sumWithMultiplier() const {
        QString mult;
        // проверка на ноль
        if( donateMultiplier > -0.001 && donateMultiplier < 0.001 )
            mult = "";
        else
            mult = QString("x%1").arg(donateMultiplier);
        return QString( "%1%2" ).arg( sum.toStr() ).arg( mult );
    }

    /**
     * Возвращает тип транзакции.
     * @param typeString тип транзакции (строковый)
     * @return тип транзакции
     */
    static TransactionType toType(const QString& typeString) {
        if( typeString == "buy" )
            return TransactionType::TYPE_BUY;
        if( typeString == "cashout" )
            return TransactionType::TYPE_CASHOUT;
        if( typeString == "donation" )
            return TransactionType::TYPE_DONATION;
        if( typeString == "partner" )
            return TransactionType::TYPE_PARTNER;
        if( typeString == "player" )
            return TransactionType::TYPE_PLAYER;
        if( typeString == "charity" )
            return TransactionType::TYPE_CHARITY;

        return TransactionType::TYPE_DONATION;
    }

    /**
     * Возвращает статус транзакции.
     * @param statusString статус транзакции (строковый)
     * @return статус транзакции
     */
    static TransactionStatus toStatus(const QString& statusString) {
        if( statusString == "success" )
            return TransactionStatus::STATUS_SUCCESS;
        if( statusString == "wait" )
            return TransactionStatus::STATUS_WAIT;
        if( statusString == "cancel" )
            return TransactionStatus::STATUS_CANCEL;
        if( statusString == "user" )
            return TransactionStatus::STATUS_USER;

        return  TransactionStatus::STATUS_SUCCESS;
    }

    /**
     * Возвращает строковое обозначение типа транзакции.
     * @param type тип транзакции
     * @return тип транзакции (строка)
     */
    static QString& fromType(const TransactionType& type) {
        static QString s;
        switch(type)
        {
            case TYPE_BUY:
                s = "buy";
                break;
            case TYPE_CASHOUT:
                s = "cashout";
                break;
            case TYPE_DONATION:
                s = "donation";
                break;
            case TYPE_PARTNER:
                s = "partner";
                break;
            case TYPE_PLAYER:
                s = "player";
                break;
            case TYPE_CHARITY:
                s = "charity";
                break;
        }
        return s;
    }

    /**
     * Возвращает строковое обозначение статуса транзакции.
     * @param status статус транзакции
     * @return статус транзакции (строка)
     */
    static QString& fromStatus(const TransactionStatus& status) {
        static QString s;
        switch(status)
        {
            case STATUS_SUCCESS:
                s = "success";
                break;
            case STATUS_WAIT:
                s = "wait";
                break;
            case STATUS_CANCEL:
                s = "cancel";
                break;
            case STATUS_USER:
                s = "user";
                break;
        }
        return s;
    }
};

/**
 * Транзакции.
 */
class Transactions : public QVector<Transaction> {
public:
    ~Transactions() {
        PROJECT_QDEBUG << "Transactions destroyed";
    }
};

PROJECT_NAMESPACE_END


Q_DECLARE_METATYPE(PROJECT::Transaction);
Q_DECLARE_METATYPE(PROJECT::Transactions);


#endif // PROJECT_TRANSACTIONS_HPP
