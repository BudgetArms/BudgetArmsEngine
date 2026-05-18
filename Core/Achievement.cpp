#include "Achievement.hpp"

#include "Core/Subject.hpp"

#include "Managers/SteamManager.hpp"

using namespace bae;


Achievement::Achievement(const std::string& id, const unsigned int eventHash, std::function<bool(Subject*)> condition) :
    m_Id{ id },
    m_EventHash{ eventHash },
    m_Condition{ std::move(condition) },
    m_bUnlocked{ false }
{
}


void Achievement::TryUnlock(const unsigned int eventHash, Subject* subject)
{
    if(m_bUnlocked || m_EventHash != eventHash)
    {
        return;
    }

    if(!m_Condition(subject))
    {
        return;
    }


    if(SteamManager::GetInstance().UnlockAchievement(m_Id))
    {
        std::cout << "Unlocked Achievement: " << m_Id << '\n';
        m_bUnlocked = true;
    }
}
