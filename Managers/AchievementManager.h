#pragma once

#include <memory>

#include "Core/Achievement.h"
#include "Singletons/Singleton.h"

#include "Core/EventTypes.h"
#include "Core/Subject.h"


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


        void Notify(EventType event, Subject* subject) override
        {
            for(const auto& achievement : m_Achievements)
            {
                if(achievement)
                    achievement->TryUnlock(event, subject);
            }
        }

    private:
        friend class Singleton<AchievementManager>;
        AchievementManager() = default;

        std::vector<std::unique_ptr<Achievement>> m_Achievements;
    };
}


