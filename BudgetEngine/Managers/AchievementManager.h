#pragma once

#include <iostream>

#include "Core/Achievement.h"
#include "Singletons/Singleton.h"

#include "Core/Events.h"
#include "Core/Subject.h"


namespace bae
{

    class AchievementManager final : public Singleton<AchievementManager>, public Observer
    {
    public:
        ~AchievementManager() = default;

        AchievementManager(const AchievementManager& other) = delete;
        AchievementManager(AchievementManager&& other) = delete;
        AchievementManager& operator=(const AchievementManager& other) = delete;
        AchievementManager& operator=(AchievementManager&& other) = delete;

        void AddAchievement(Achievement achievement)
        {
            m_Achievements.emplace_back(achievement);
        }


        virtual void Notify(Event event, Subject* subject) override
        {
            for (auto& achievement : m_Achievements)
                achievement.TryUnlock(event, subject);

            return;
        }


    private:
        friend class Singleton<AchievementManager>;
        AchievementManager() = default;

        std::vector<Achievement> m_Achievements;


    };


}
