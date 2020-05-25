//
// Created by cedex on 12/12/2019.
//

#ifndef PROJECT_MONEY_HPP
#define PROJECT_MONEY_HPP

#include <cstdint>
#include <QString>
#include <QMetaClassInfo>

#include "Global.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Хранитель денег.
 * Отдельно хранит целые деньги и копейки
 * чтобы не терялась точность
 */
class Money {

/* ############################################################################################## */
public: /* Конструкторы / деструктор */
/* ############################################################################################## */

    /**
     * Дефолтный конструктор.
     */
    Money();
    /**
     * Еще немного конструкторов.
     */
    Money(uint64_t intPart, uint16_t fractPart);
    explicit Money(double value);
    explicit Money(const QString& value);

    /// Сложение
    Money operator+(const Money& right) const {
        return add( *this, right);
    }
    /// Вычитание (возвращает 0, если вычитание будет отрицательным)
    Money operator-(const Money& right) const {
        return subtract( *this, right);
    }

    Money& operator+=(const Money& right) {
        *this = add(*this, right);
        return *this;
    }

    bool operator<(const Money& right) const {
        return isLess( *this, right);
    }


/* ############################################################################################## */
public: /* Методы */
/* ############################################################################################## */

    uint64_t    intPart() const;
    void        setIntPart(const uint64_t& intPart);

    uint16_t    fractPart() const;
    void        setFractPart(const uint16_t& fractPart);

    /**
     * Возвращает сумму в строковом виде по типу 123.45.
     * @return Сумма в строковом виде
     */
    QString     toStr() const;

    /**
     * Возвращает сумму в double.
     * @return Сумма в double
     */
    double      toDouble() const;

    /**
     * Статический метод - преобразует double в Money.
     */
    static Money fromDouble(double value);

    /**
     * Проверяет есть ли деньги :).
     * @return Факт отсутствия денег
     */
    bool        isZero() const;


/* ############################################################################################## */
private: /* Приваты */
/* ############################################################################################## */

    uint64_t    m_intPart;          /// Целая часть суммы
    uint16_t    m_fractPart;        /// Дробная часть суммы (копейки)

    static uint64_t    extractIntPart(double value);
    static uint16_t    extractFractPart(double value);

    Money       add(const Money& m1, const Money& m2) const;
    Money       subtract(const Money& m1, const Money& m2) const;
    bool        isLess(const Money& m1, const Money& m2) const;
};

PROJECT_NAMESPACE_END


Q_DECLARE_METATYPE(PROJECT::Money);


#endif //PROJECT_MONEY_HPP
