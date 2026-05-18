#pragma once

#include <memory>

#include "Core/Achievement.hpp"
#include "Singletons/Singleton.hpp"

#include "Core/Subject.hpp"


namespace bae
{
    class AchievementManager final : public Singleton<AchievementManager>, public Observer
    {
    public:

        void AddAchievement(std::unique_ptr<Achievement> achievement)
        {
            m_Achievements.emplace_back(std::move(achievement));
        }


        void Notify(const unsigned int eventHash, Subject* subject) override
        {
            for(const auto& achievement : m_Achievements)
            {
                if(achievement)
                    achievement->TryUnlock(eventHash, subject);
            }
        }

    private:
        friend class Singleton;
        AchievementManager() = default;
        ~AchievementManager() override = default;

        std::vector<std::unique_ptr<Achievement>> m_Achievements;
    };
}


