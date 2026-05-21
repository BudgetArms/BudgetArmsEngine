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

        Achievement(const Achievement&)            = delete;
        Achievement(Achievement&&)                 = delete;
        Achievement& operator=(const Achievement&) = delete;
        Achievement& operator=(Achievement&&)      = delete;


        void TryUnlock(unsigned int eventHash, Subject* subject);

    private:
        std::string m_Id;
        unsigned int m_EventHash;
        std::function<bool(Subject*)> m_Condition;
        bool m_bUnlocked;
    };
}


