#pragma once

#include "IUserTierProvider.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

class FakeUserTierProvider : public IUserTierProvider
{
public:
    void setTier(const std::string& userId, UserTier tier)
    {
        m_tiers[userId] = tier;
    }

    UserTier getUserTier(const std::string& userId) const override
    {
        const auto it = m_tiers.find(userId);
        if (it == m_tiers.end())
        {
            throw std::runtime_error("Unknown user");
        }

        return it->second;
    }

private:
    std::unordered_map<std::string, UserTier> m_tiers;
};
