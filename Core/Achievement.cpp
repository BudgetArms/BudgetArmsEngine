#include "Achievement.hpp"

#include "Core/Logger.hpp"
#include "Core/Subject.hpp"

#ifdef STEAMWORKS_ENABLED
#include "Managers/SteamManager.hpp"
#endif


using namespace bae;


Achievement::Achievement(const std::string& id, const unsigned int eventHash, std::function<bool(Subject*)> condition) :
    m_Id{ id },
    m_EventHash{ eventHash },
    m_Condition{ std::move(condition) }
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


    #ifdef STEAMWORKS_ENABLED
    if(SteamManager::GetInstance().UnlockAchievement(m_Id))
    {
        Logger::LogNormal("Unlocked Achievement: " + m_Id);
        m_bUnlocked = true;
    }
    #endif
}
