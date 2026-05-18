#pragma once

#include <string>

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#include "Singletons/Singleton.hpp"


namespace bae
{
    class SteamManager final : public Singleton<SteamManager>
    {
    public:
        bool Initialize(uint32_t appId = 0);
        void Shutdown();
        void Update() const;

        [[nodiscard]] bool UnlockAchievement(const std::string& achievementId) const;
        [[nodiscard]] bool IsAchievementUnlocked(const std::string& achievementId) const;
        void ResetAllAchievements() const;

        [[nodiscard]] bool GetStat(const std::string& statName, int32_t& outValue) const;
        [[nodiscard]] bool GetStat(const std::string& statName, float& outValue) const;
        [[nodiscard]] bool SetStat(const std::string& statName, int32_t value) const;
        [[nodiscard]] bool SetStat(const std::string& statName, float value) const;

        void UploadScore(const std::string& leaderboardName, int32_t score) const;

        [[nodiscard]] std::string GetPlayerName() const;
        [[nodiscard]] uint64_t GetSteamID() const;

        [[nodiscard]] bool IsInitialized() const { return m_bInitialized; }
        [[nodiscard]] bool IsSteamRunning() const;

    private:
        friend class Singleton;
        SteamManager()           = default;
        ~SteamManager() override = default;

        bool m_bInitialized{ false };
    };
}


