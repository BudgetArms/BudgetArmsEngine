#pragma once

#include <iostream>
#include <memory>

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

        void AddAchievement(std::unique_ptr<Achievement> achievement)
        {
            m_Achievements.emplace_back(std::move(achievement));
        }


        virtual void Notify(Event event, Subject* subject) override
        {
            for (auto& achievement : m_Achievements)
                achievement->TryUnlock(event, subject);

            return;
        }


    private:
        friend class Singleton<AchievementManager>;
        AchievementManager() = default;

        std::vector<std::unique_ptr<Achievement>> m_Achievements;


    };


}
