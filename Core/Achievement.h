﻿#pragma once

#include <functional> 

#include "Core/Observer.h"
#include "Core/Subject.h"


namespace bae
{
    class AchievementManager;

    class Achievement final
    {
    public:
        Achievement(const std::string& id, EventType event, std::function<bool(Subject*)> condition);
        ~Achievement() = default;

        Achievement(const Achievement& other) = delete;
        Achievement(Achievement&& other) = delete;
        Achievement& operator=(const Achievement& other) = delete;
        Achievement& operator=(Achievement&& other) = delete;


        void TryUnlock(EventType event, Subject* subject);


    private:
        std::string m_Id;
        EventType m_Event;
        std::function<bool(Subject*)> m_Condition;
        bool m_bUnlocked;


    };

}


