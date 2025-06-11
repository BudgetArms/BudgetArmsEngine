#pragma once

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#include "Singletons/Singleton.h"
#include "Core/Observer.h"
#include "Core/Subject.h"
#include "Core/EventTypes.h"

#include <string>
#include <functional>


namespace bae
{
    class SteamManager final : public Singleton<SteamManager>
    {
    public:
        ~SteamManager() = default;

        SteamManager(const SteamManager& other) = delete;
        SteamManager(SteamManager&& other) = delete;
        SteamManager& operator=(const SteamManager& other) = delete;
        SteamManager& operator=(SteamManager&& other) = delete;

        bool Initialize(uint32_t appId = 0);
        void Shutdown();
        void Update();

        bool UnlockAchievement(const std::string& achievementId);
        bool IsAchievementUnlocked(const std::string& achievementId);
        void ResetAllAchievements();

        bool SetStat(const std::string& statName, int32_t value);
        bool SetStat(const std::string& statName, float value);
        bool GetStat(const std::string& statName, int32_t& outValue);
        bool GetStat(const std::string& statName, float& outValue);

        void UploadScore(const std::string& leaderboardName, int32_t score);

        std::string GetPlayerName();
        uint64_t GetSteamID();

        bool IsInitialized() const { return m_bInitialized; }
        bool IsSteamRunning() const;


    private:
        friend class Singleton<SteamManager>;
        SteamManager() = default;

        bool m_bInitialized{ false };



    };

}