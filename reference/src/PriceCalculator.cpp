#include "PriceCalculator.h"

#include <cmath>
#include <stdexcept>

PriceCalculator::PriceCalculator(const IUserTierProvider& userTierProvider)
    : m_userTierProvider(userTierProvider)
{
}

double PriceCalculator::calculateFinalPrice(const std::string& userId, double basePrice) const
{
    // Improved behavior: fail fast for invalid prices instead of calculating nonsense values.
    if (basePrice < 0.0 || std::isnan(basePrice) || std::isinf(basePrice))
    {
        throw std::invalid_argument("basePrice must be a finite non-negative value");
    }

    const auto tier = m_userTierProvider.getUserTier(userId);
    const auto discount = getDiscountForTier(tier);

    return basePrice - (basePrice * discount);
}

double PriceCalculator::getDiscountForTier(UserTier tier) const
{
    // Refactor improvement: keep tier rules in one place to simplify future changes.
    switch (tier)
    {
        case UserTier::Regular:
            return 0.0;
        case UserTier::Premium:
            return 0.10;
        case UserTier::Vip:
            // Fixed from the buggy candidate implementation: VIP discount is 20%, not 25%.
            return 0.20;
    }

    throw std::runtime_error("Unsupported user tier");
}
