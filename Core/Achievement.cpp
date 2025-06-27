#include "Achievement.h"


#include "Core/Subject.h"
#include "Core/EventTypes.h"

#include "Managers/SteamManager.h"

using namespace bae;


Achievement::Achievement(const std::string& id, EventType event, std::function<bool(Subject*)> condition) :
    m_Id{ id },
    m_Event{ event },
    m_Condition{ std::move(condition) },
    m_bUnlocked{ false }
{
};


void Achievement::TryUnlock(EventType event, Subject* subject)
{
    if (m_bUnlocked || m_Event != event)
        return;

    if (!m_Condition(subject))
        return;


    if (SteamManager::GetInstance().UnlockAchievement(m_Id))
    {
        std::cout << "Unlocked Achievement: " << m_Id << '\n';
        m_bUnlocked = true;
    }

}
