#pragma once

#include <memory>

#include "Core/Achievement.hpp"
#include "Singletons/Singleton.hpp"

#include "Core/EventTypes.hpp"
#include "Core/Subject.hpp"


namespace bae
{
    class AchievementManager final : public Singleton<AchievementManager>, public Observer
    {
    public:
        ~AchievementManager() override = default;

        AchievementManager(const AchievementManager& other)            = delete;
        AchievementManager(AchievementManager&& other)                 = delete;
        AchievementManager& operator=(const AchievementManager& other) = delete;
        AchievementManager& operator=(AchievementManager&& other)      = delete;

        void AddAchievement(std::unique_ptr<Achievement> achievement)
        {
            m_Achievements.emplace_back(std::move(achievement));
        }


        void Notify(const EventType event, Subject* subject) override
        {
            for(const auto& achievement : m_Achievements)
            {
                if(achievement)
                    achievement->TryUnlock(event, subject);
            }
        }

    private:
        friend class Singleton;
        AchievementManager() = default;

        std::vector<std::unique_ptr<Achievement>> m_Achievements;
    };
}


