#pragma once

#include <iostream>
#include <memory>
#include "Core/SoundSystem.h"
#include "Core/NullSoundSystem.h"


namespace bae
{
    class LoggingSoundSystem : public SoundSystem
    {
    public:
        LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem);
        virtual ~LoggingSoundSystem();


        virtual int LoadSound(const std::string& path) override;

        virtual void Play(int soundId, float volume) override;
        virtual void Stop(int soundId) override;

        virtual void Resume(int soundId) override;
        virtual void Pause(int soundId) override;

        virtual void Mute(int soundId) override;
        virtual void UnMute(int soundId) override;

        virtual bool IsLoaded(int soundId) override;
        virtual bool IsPlaying(int soundId) override;

        virtual bool IsPaused(int soundId) override;
        virtual bool IsMuted(int soundId) override;

        virtual void StopAllSounds() override;
        virtual void MuteAllSounds() override;
        virtual void UnMuteAllSounds() override;


    private:
        std::unique_ptr<SoundSystem> m_RealSoundSystem;


    };
}


