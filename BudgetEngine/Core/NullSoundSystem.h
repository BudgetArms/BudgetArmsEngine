#pragma once

#include "Core/SoundSystem.h"

namespace bae
{
    class SoundSystem;
    class NullSoundSystem : public SoundSystem
    {
    public:
        virtual ~NullSoundSystem() override {};

        virtual int LoadSound(const std::string&) override { return 0; };

        virtual void Play(int, float) override {};
        virtual void Stop(int) override {};
        virtual void Mute(int) override {};

        virtual bool IsLoaded(int) override { return 0; };
        virtual bool IsPlaying(int) override { return 0; };

        virtual void StopAllSounds() override {};
        virtual void MuteAllSounds() override {};

    };
}


