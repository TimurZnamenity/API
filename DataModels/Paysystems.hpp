//
// Created by cedex on 01/20/2020.
//

#ifndef PROJECT_PAYSYSTEMS_HPP
#define PROJECT_PAYSYSTEMS_HPP

#include <QString>
#include <QMetaClassInfo>
#include <QHash>
#include "Global.hpp"
#include "Money.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Платежная система со всеми своими параметрами.
 */
struct Paysystem
{
    /// Имя.
    QString         name;
    /// Максимальная сумма вывода
    Money           max;
    /// Минимальная сумма вывода
    Money           min;
    /// Комиссия (в %).
    double          commission;
    /// Фиксированная комиссия (в деньгах).
    Money           staticCommission;
    /// Кошелек.
    QString         placeholder;
    /// Иконка (ссылка).
    QString         iconUrl;
    /// Метод пакует обе комисии в одну строку
    QString commissionsToString() {
        QString result;
        if( (commission < 0.0001) && staticCommission.isZero() )
            // обе комиссии - нули, пишем 0%
            result = QString("0 %");
        if( commission < 0.0001 )
            // это ноль - теперь только фиксированная комиссия
            result = QString("%1 руб").arg( staticCommission.toStr() );
        if( staticCommission.isZero() )
            // это ноль - теперь только проценты
            result = QString("%1 %").arg( commission );
        if( (commission > 0.0001) && !staticCommission.isZero() )
            // обе комиссии - не нули, пишем обе комиссии
            result = QString("%1 % + %2 руб").arg( commission ).arg(staticCommission.toStr());
        return result;
    }
    /// Рассчитывает коммиссию исходя из заданной суммы
    Money calcCommission(const Money& sum) {
        Money result;
        // добавляем стат. комиссию
        result += staticCommission;
        // добавляем проценты
        result += Money( sum.toDouble() * commission / 100 );
        return result;
    }
    /// Рассчитывает Итого исходя из заданной суммы, зная свои комиссии
    Money calcTotal(const Money& sum) {
        return sum + calcCommission(sum);
    }
};

/**
 * Перечень всех платежных систем, адресуемый по ключу-строке.
 */
class Paysystems : public QHash<QString,Paysystem> {
public:
    ~Paysystems() {
        PROJECT_QDEBUG << "Paysystems destroyed";
    }
};


PROJECT_NAMESPACE_END


Q_DECLARE_METATYPE(PROJECT::Paysystems);

#endif //PROJECT_PAYSYSTEMS_HPP
