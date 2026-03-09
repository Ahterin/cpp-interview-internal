#pragma once

#include "IUserTierProvider.h"

#include <string>

class PriceCalculator
{
public:
    explicit PriceCalculator(const IUserTierProvider& userTierProvider);

    double calculateFinalPrice(const std::string& userId, double basePrice) const;

private:
    // Refactor improvement: isolate tier-to-discount rules from price calculation flow.
    double getDiscountForTier(UserTier tier) const;
    const IUserTierProvider& m_userTierProvider;
};
