#pragma once

#include <string>

enum class UserTier
{
    Regular,
    Premium,
    VIP
};

class IUserTierProvider
{
public:
    virtual ~IUserTierProvider() = default;
    virtual UserTier getUserTier(const std::string& userId) const = 0;
};
