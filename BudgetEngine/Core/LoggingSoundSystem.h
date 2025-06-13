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
        virtual void Mute(int soundId) override;

        virtual bool IsLoaded(int soundId) override;
        virtual bool IsPlaying(int soundId) override;

        virtual void StopAllSounds() override;
        virtual void MuteAllSounds() override;


    private:
        std::unique_ptr<SoundSystem> m_RealSoundSystem;


    };
}


