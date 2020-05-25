//
// Created by cedex on 12/11/2019.
//

#ifndef PROJECT_USER_HPP
#define PROJECT_USER_HPP

#include <QString>
#include <QMetaClassInfo>
#include "Global.hpp"
#include "Money.hpp"


PROJECT_NAMESPACE_BEGIN


/**
 * Все данные пользователя.
 */
struct User
{
    /// ID.
    uint64_t        id;
    /// Имя.
    QString         name;
    /// ссылка на аватар.
    QString         avatar;
    /// баланс или остаток денег на счету.
    Money       cash;

    ~User() {
        PROJECT_QDEBUG << "User destroyed";
    }
};

PROJECT_NAMESPACE_END


Q_DECLARE_METATYPE(PROJECT::User);

#endif //PROJECT_USER_HPP
