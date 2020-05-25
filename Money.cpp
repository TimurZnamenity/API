//
// Created by cedex on 12/12/2019.
//

#include <iostream>
#include "Money.hpp"


using namespace PROJECT;


/* ############################################################################################## */
/* Конструкторы / деструктор */
/* ############################################################################################## */

Money::Money()
{
    m_intPart   = static_cast<uint64_t> ( 0 );
    m_fractPart = static_cast<uint16_t> ( 0 );
}


/* ---------------------------------------------------------------------------------------------- */
Money::Money(uint64_t intPart, uint16_t fractPart)
{
    if (fractPart > 99)
    {
        return;
    }
    m_intPart   = intPart;
    m_fractPart = fractPart;
}


/* ---------------------------------------------------------------------------------------------- */
Money::Money(double value)
{
    m_intPart   = extractIntPart  ( value );
    m_fractPart = extractFractPart( value );
}


/* ---------------------------------------------------------------------------------------------- */
Money::Money(const QString& value)
{
    // преобразуем в double
    bool flag;
    double result = value.toDouble(&flag);

    if( !flag )
    {
        // ошибка в числе
        result = 0;                 // вернем 0
    }

    // проверка на отрицательность
    // не допускаем отрицательных денег
    if( result < 0 )
        result = -result;

    // сохраняем деньги
    m_intPart   = extractIntPart  ( result );
    m_fractPart = extractFractPart( result );
}


/* ############################################################################################## */
/* Методы */
/* ############################################################################################## */

uint64_t Money::intPart() const
{
    return m_intPart;
}


/* ---------------------------------------------------------------------------------------------- */
void Money::setIntPart(const uint64_t& intPart)
{
    m_intPart = intPart;
}


/* ---------------------------------------------------------------------------------------------- */
uint16_t Money::fractPart() const
{
    return m_fractPart;
}


/* ---------------------------------------------------------------------------------------------- */
void Money::setFractPart(const uint16_t& fractPart)
{
    m_fractPart = fractPart;
}


/* ---------------------------------------------------------------------------------------------- */
QString Money::toStr() const
{
    return QString("%1.%2").arg( m_intPart ).arg( m_fractPart, 2, 10, static_cast<QChar>('0') );
}


/* ---------------------------------------------------------------------------------------------- */
double Money::toDouble() const
{
    double result = static_cast<double>( fractPart() ) / 100.0;
    result       += static_cast<double>( intPart() );
    return result;
}


/* ---------------------------------------------------------------------------------------------- */
Money Money::fromDouble(double value)
{
    Money result;
    result.setIntPart  ( extractIntPart  (value) );
    result.setFractPart( extractFractPart(value) );
    return result;
}


/* ---------------------------------------------------------------------------------------------- */
bool Money::isZero() const
{
    return (m_fractPart == 0) && (m_intPart == 0);
}


/* ---------------------------------------------------------------------------------------------- */
uint64_t Money::extractIntPart(double value)
{
    return static_cast<uint64_t>(value);
}


/* ---------------------------------------------------------------------------------------------- */
uint16_t Money::extractFractPart(double value)
{
    uint64_t intPart = extractIntPart(value);
    double fract = (value - intPart) * 100 + 0.5;
    return static_cast<uint16_t>(fract);
}


/* ---------------------------------------------------------------------------------------------- */
Money Money::add(const Money& m1, const Money& m2) const
{
    Money result;
    // сначала копейки с проверкой переноса
    result.m_fractPart = m1.m_fractPart + m2.m_fractPart;
    if( result.m_fractPart >= 100 )
    {
        // есть перенос
        result.m_fractPart -= 100;
        result.m_intPart = 1;
    }
    // теперь рубли (добавляем к переносу, если он был)
    result.m_intPart += m1.m_intPart + m2.m_intPart;

    return result;
}


/* ---------------------------------------------------------------------------------------------- */
Money Money::subtract(const Money& m1, const Money& m2) const
{
    // если m1 меньше m2, возвращаем 0
    if( isLess(m1,m2) )
        return Money();

    // теперь вычитаем - сначала вычитаем копейки с проверкой займа
    Money result;
    bool borrow = m1.m_fractPart < m2.m_fractPart;
    if(borrow)
        result.m_fractPart = (100 + m1.m_fractPart) - m2.m_fractPart;
    else
        result.m_fractPart = m1.m_fractPart - m2.m_fractPart;

    // потом вычитаем рубли с учетом займа
    result.m_intPart = m1.m_intPart - m2.m_intPart;
    if( borrow )
        --result.m_intPart;

    return result;
}

/* ---------------------------------------------------------------------------------------------- */
bool Money::isLess(const Money& m1, const Money& m2) const
{
    // если рубли первого меньше, то возвращаем 0
    if( m1.m_intPart < m2.m_intPart )
        return true;
    // если рубли равны, но копейки первого меньше, то возвращаем 0
    if( (m1.m_intPart == m2.m_intPart) && (m1.m_fractPart < m2.m_fractPart) )
        return true;
    return false;
}

