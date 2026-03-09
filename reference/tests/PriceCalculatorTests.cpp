#include "FakeUserTierProvider.h"
#include "PriceCalculator.h"

#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>

namespace {
constexpr double kEpsilon = 1e-9;
}  // namespace

TEST(PriceCalculatorTests, RegularUserKeepsOriginalPrice) {
    FakeUserTierProvider provider;
    provider.setTier("alice", UserTier::Regular);

    const PriceCalculator calculator(provider);
    const double finalPrice = calculator.calculateFinalPrice("alice", 100.0);

    EXPECT_NEAR(finalPrice, 100.0, kEpsilon);
}

TEST(PriceCalculatorTests, PremiumUserGetsTenPercentDiscount) {
    FakeUserTierProvider provider;
    provider.setTier("bob", UserTier::Premium);

    const PriceCalculator calculator(provider);
    const double finalPrice = calculator.calculateFinalPrice("bob", 100.0);

    EXPECT_NEAR(finalPrice, 90.0, kEpsilon);
}

// Added to catch the original VIP regression (candidate branch used 25%).
TEST(PriceCalculatorTests, VIPUserGetsTwentyPercentDiscount) {
    FakeUserTierProvider provider;
    provider.setTier("charlie", UserTier::VIP);

    const PriceCalculator calculator(provider);
    const double finalPrice = calculator.calculateFinalPrice("charlie", 100.0);

    EXPECT_NEAR(finalPrice, 80.0, kEpsilon);
}

TEST(PriceCalculatorTests, ZeroPriceForPremiumUser) {
    FakeUserTierProvider provider;
    provider.setTier("zero", UserTier::Premium);

    const PriceCalculator calculator(provider);
    const double finalPrice = calculator.calculateFinalPrice("zero", 0.0);

    EXPECT_NEAR(finalPrice, 0.0, kEpsilon);
}

// Added validation coverage for rejected input values.
TEST(PriceCalculatorTests, NegativePriceThrows) {
    FakeUserTierProvider provider;
    provider.setTier("eve", UserTier::Regular);

    const PriceCalculator calculator(provider);

    EXPECT_THROW(calculator.calculateFinalPrice("eve", -10.0), std::invalid_argument);
}

TEST(PriceCalculatorTests, NanPriceThrows) {
    FakeUserTierProvider provider;
    provider.setTier("nan-user", UserTier::Regular);

    const PriceCalculator calculator(provider);

    EXPECT_THROW(
        calculator.calculateFinalPrice("nan-user", std::numeric_limits<double>::quiet_NaN()),
        std::invalid_argument);
}

TEST(PriceCalculatorTests, PositiveInfinityPriceThrows) {
    FakeUserTierProvider provider;
    provider.setTier("inf-user", UserTier::Regular);

    const PriceCalculator calculator(provider);

    EXPECT_THROW(
        calculator.calculateFinalPrice("inf-user", std::numeric_limits<double>::infinity()),
        std::invalid_argument);
}

TEST(PriceCalculatorTests, NegativeInfinityPriceThrows) {
    FakeUserTierProvider provider;
    provider.setTier("neg-inf-user", UserTier::Regular);

    const PriceCalculator calculator(provider);

    EXPECT_THROW(
        calculator.calculateFinalPrice("neg-inf-user", -std::numeric_limits<double>::infinity()),
        std::invalid_argument);
}

TEST(PriceCalculatorTests, UnknownUserThrows) {
    FakeUserTierProvider provider;
    const PriceCalculator calculator(provider);

    EXPECT_THROW(calculator.calculateFinalPrice("missing-user", 100.0), std::runtime_error);
}
