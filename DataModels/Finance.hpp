//
// Created by cedex on 01/20/2020.
//

#ifndef PROJECT_FINANCE_HPP
#define PROJECT_FINANCE_HPP


#include <cstdint>
#include "Global.hpp"
#include "Money.hpp"


PROJECT_NAMESPACE_BEGIN

/**
 * Все что касается денег.
 */
struct Finance
{
    /// баланс или остаток денег на счету.
    Money       cash;
    /// кол-во донатов.
    uint64_t    donationAmount;
    /// сумма пришедших денег.
    Money       cashIn;
    /// сумма выведенных денег.
    Money       cashOut;
};

PROJECT_NAMESPACE_END


Q_DECLARE_METATYPE( PROJECT::Finance );

#endif //PROJECT_FINANCE_HPP
