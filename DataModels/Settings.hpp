//
// Created by cedex on 01/20/2020.
//

#ifndef PROJECT_SETTINGS_HPP
#define PROJECT_SETTINGS_HPP


#include <cstdint>
#include <QVector>
#include "Global.hpp"
#include "Money.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Кошелек.
 */
struct Wallet
{
    /// Платежная система.
    QString     key;
    /// Кошелек (аккаунт).
    QString     value;
};


/**
 * Список кошельков.
 */
class Wallets : public QVector<Wallet> {};


/**
 * Все настройки пользователя.
 * В том числе кошельки
 */
struct Settings
{
    /// Валюта (пока не используется)
    QString currency;
    /// Кошельки
    Wallets wallets;
};


PROJECT_NAMESPACE_END


Q_DECLARE_METATYPE( PROJECT::Wallet );
Q_DECLARE_METATYPE( PROJECT::Wallets );
Q_DECLARE_METATYPE( PROJECT::Settings );


#endif //PROJECT_SETTINGS_HPP
