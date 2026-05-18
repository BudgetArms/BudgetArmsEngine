#include "SteamManager.hpp"

#include <iostream>


using namespace bae;


// Todo: Remove this from the BAE and into the Game Level
bool SteamManager::Initialize(const uint32_t appId)
{
    if(m_bInitialized)
    {
        return true;
    }

    if(SteamAPI_RestartAppIfNecessary(appId))
    {
        return false;
    }

    if(!SteamAPI_Init())
    {
        std::cerr << "Steam failed to initialize" << '\n';
        std::cerr << "Make sure Steam is running" << '\n';
        return false;
    }

    std::cout << "Steam Initialize" << '\n';
    m_bInitialized = true;

    // TODO: check if this is true
    // This should now be done automatically
    // SteamUserStats()->RequestCurrentStats();

    return true;
}

void SteamManager::Shutdown()
{
    if(!m_bInitialized)
    {
        return;
    }

    SteamAPI_Shutdown();
    m_bInitialized = false;
}

void SteamManager::Update() const
{
    if(m_bInitialized)
    {
        // SteamAPI_RunCallbacks();
    }
}


bool SteamManager::UnlockAchievement(const std::string& achievementId) const
{
    if(!m_bInitialized)
    {
        return false;
    }


    if(SteamUserStats()->SetAchievement(achievementId.c_str()))
    {
        return SteamUserStats()->StoreStats();
    }

    return false;
}

bool SteamManager::IsAchievementUnlocked(const std::string& achievementId) const
{
    if(!m_bInitialized)
    {
        return false;
    }


    bool achieved = false;
    SteamUserStats()->GetAchievement(achievementId.c_str(), &achieved);

    return achieved;
}

void SteamManager::ResetAllAchievements() const // Debug only
{
    if(!m_bInitialized)
    {
        return;
    }

    std::cout << "Steam Achievements Reset" << '\n';
    SteamUserStats()->ResetAllStats(true);
    SteamUserStats()->StoreStats();
}

bool SteamManager::GetStat(const std::string& statName, int32_t& outValue) const
{
    if(!m_bInitialized)
    {
        return false;
    }

    return SteamUserStats()->GetStat(statName.c_str(), &outValue);
}

bool SteamManager::GetStat(const std::string& statName, float& outValue) const
{
    if(!m_bInitialized)
    {
        return false;
    }

    return SteamUserStats()->GetStat(statName.c_str(), &outValue);
}

bool SteamManager::SetStat(const std::string& statName, const int32_t value) const
{
    if(!m_bInitialized)
    {
        return false;
    }

    return SteamUserStats()->SetStat(statName.c_str(), value);
}

bool SteamManager::SetStat(const std::string& statName, const float value) const
{
    if(!m_bInitialized)
    {
        return false;
    }

    return SteamUserStats()->SetStat(statName.c_str(), value);
}

// Todo: change this to update the score
//void SteamManager::UploadScore(const std::string& leaderboardName, int32_t score)
void SteamManager::UploadScore(const std::string&, int32_t) const
{
    if(!m_bInitialized)
    {
        return;
    }
}

std::string SteamManager::GetPlayerName() const
{
    if(!m_bInitialized)
    {
        return "Default";
    }

    return SteamFriends()->GetPersonaName();
}

uint64_t SteamManager::GetSteamID() const
{
    if(!m_bInitialized)
    {
        return 0;
    }

    return SteamUser()->GetSteamID().ConvertToUint64();
}


bool SteamManager::IsSteamRunning() const
{
    if(!m_bInitialized)
    {
        return false;
    }

    return SteamAPI_IsSteamRunning();
}


