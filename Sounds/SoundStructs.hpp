#pragma once
#include <functional>
#include <stdexcept>

#include "Core/HelperFunctions.hpp"


namespace bae
{
    struct SoundID
    {
        int ID;
    };

    struct ActiveSoundID
    {
        explicit ActiveSoundID(const int id) :
            ID{ id }
        {
        }

        int ID{ -1 };

        static ActiveSoundID GenerateActiveSoundID()
        {
            return ActiveSoundID(GetRandomNumber());
        }
    };


    constexpr bool operator==(const SoundID& lhs, const SoundID& rhs)
    {
        return lhs.ID == rhs.ID;
    }

    constexpr bool operator==(const ActiveSoundID& lhs, const ActiveSoundID& rhs)
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


