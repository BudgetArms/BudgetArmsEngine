#pragma once

#include <memory>
#include <iostream>
#include "Core/SoundSystem.h"


namespace bae
{
    class SdlSoundSystem : public SoundSystem
    {
    public:
        SdlSoundSystem();
        virtual ~SdlSoundSystem();

        virtual int LoadSound(const std::string& path) override;

        virtual void Play(int soundId, float volume) override;
        virtual void Stop(int soundId) override;
        virtual void Mute(int soundId) override;

        virtual bool IsLoaded(int soundId) override;
        virtual bool IsPlaying(int soundId) override;


        virtual void StopAllSounds() override;
        virtual void MuteAllSounds() override;


    private:
        // Pimple implmentation
        class Impl;
        std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };


    };
}

