#pragma once

#include <unordered_map>

// I want to have a sub namespace Game::Sound
// the same for bae::Sound, and such, but there is no time for that
namespace Game
{
    enum class SoundEvents
    {
        QbertHurt,
        QbertJump,
        Coin,
        Tune,
        Fall,
        Fall2,
        Fall3,
        Fall4,

    };

    static std::unordered_map<SoundEvents, int> g_sSoundEvents;


};


