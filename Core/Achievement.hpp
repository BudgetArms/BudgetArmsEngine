#pragma once

#include <functional>

#include "Core/Subject.hpp"


namespace bae
{
    class AchievementManager;

    class Achievement final
    {
    public:
        Achievement(const std::string& id, unsigned int eventHash, std::function<bool(Subject*)> condition);
        ~Achievement() = default;

        Achievement(const Achievement& other)            = delete;
        Achievement(Achievement&& other)                 = delete;
        Achievement& operator=(const Achievement& other) = delete;
        Achievement& operator=(Achievement&& other)      = delete;


        void TryUnlock(unsigned int eventHash, Subject* subject);

    private:
        std::string m_Id;
        unsigned int m_EventHash;
        std::function<bool(Subject*)> m_Condition;
        bool m_bUnlocked;
    };
}


