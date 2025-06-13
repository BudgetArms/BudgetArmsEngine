#pragma once

#include <string>
#include <memory>
#include "Core/AudioClip.h"


namespace bae
{
    class SdlAudioClip : public AudioClip
    {
    public:
        SdlAudioClip(const std::string& path);
        ~SdlAudioClip(); // empty, needed for Pimpl

        virtual void Play() override;
        virtual void Stop() override;
        virtual void Mute() override;

        virtual bool IsPlaying() override;
        virtual bool IsLoaded() override;

        virtual float GetVolume() override;
        virtual void SetVolume(float volume) override;


    private:
        class Impl;
        std::unique_ptr<Impl> m_Pimpl;


    };
}


