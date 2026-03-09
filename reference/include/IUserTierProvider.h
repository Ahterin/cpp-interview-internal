#pragma once

#include <string>

enum class UserTier
{
    Regular,
    Premium,
    Vip
};

class IUserTierProvider
{
public:
    virtual ~IUserTierProvider() = default;
    virtual UserTier getUserTier(const std::string& userId) const = 0;
};
