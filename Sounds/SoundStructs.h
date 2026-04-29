#pragma once
#include <functional>
#include <stdexcept>

#include "Core/HelperFunctions.h"


namespace bae
{
    struct SoundID
    {
        int ID;
    };

    struct ActiveSoundID
    {
        int ID{ -1 };

        static ActiveSoundID GenerateActiveSoundID()
        {
            return ActiveSoundID{ .ID = GetRandomNumber() };
        }
    };

    enum class SoundEventType
    {
        Play,
        Stop,
        Resume,
        Pause,
        Mute,
        UnMute,
        SetVolume,
        StopAll,
        ResumeAll,
        PauseAll,
        MuteAll,
        UnMuteAll,
        SetVolumeAll,
    };

    [[nodiscard]] constexpr bool IsAudioClipNeededForSoundEventType(const SoundEventType eventType)
    {
        // Todo: check how normal this include is, because it looks pretty fucked, but it makes

        switch(eventType)
        {
            case SoundEventType::Play:
            case SoundEventType::Stop:
            case SoundEventType::Resume:
            case SoundEventType::Pause:
            case SoundEventType::Mute:
            case SoundEventType::UnMute:
            case SoundEventType::SetVolume:
                return true;

            case SoundEventType::StopAll:
            case SoundEventType::ResumeAll:
            case SoundEventType::PauseAll:
            case SoundEventType::MuteAll:
            case SoundEventType::UnMuteAll:
            case SoundEventType::SetVolumeAll:
                return false;
        }

        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! This line should never be reached"));
    }

    struct SoundEventData
    {
        SoundEventType Type{};
        SoundID SoundID{ -1 };
        ActiveSoundID ActiveSoundID{ -1 };
        float Volume{ 1 };
    };


    constexpr bool operator==(const SoundID& lhs, const SoundID& rhs)
    {
        return lhs.ID == rhs.ID;
    }

    inline bool operator==(const ActiveSoundID& lhs, const ActiveSoundID& rhs)
    {
        return lhs.ID == rhs.ID;
    }
}


template<>
struct std::hash<bae::SoundID>
{
    std::size_t operator()(const bae::SoundID& soundId) const noexcept
    {
        return std::hash<int>{}(soundId.ID);
    }
};

template<>
struct std::hash<bae::ActiveSoundID>
{
    std::size_t operator()(const bae::ActiveSoundID& activeSoundId) const noexcept
    {
        return std::hash<int>{}(activeSoundId.ID);
    }
};


