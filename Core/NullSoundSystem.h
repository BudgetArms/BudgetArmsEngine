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

        virtual void Resume(int) override {};
        virtual void Pause(int) override {};

        virtual void Mute(int) override {};
        virtual void UnMute(int) override {};

        virtual bool IsLoaded(int) override { return 0; };
        virtual bool IsPlaying(int) override { return 0; };

        virtual bool IsPaused(int) override { return 0; };
        virtual bool IsMuted(int) override { return 0; };


        virtual void StopAllSounds() override {};
        virtual void MuteAllSounds() override {};
        virtual void UnMuteAllSounds() override {};

    };
}


