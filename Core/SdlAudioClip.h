#pragma once

#include <string>
#include <memory>
#include "Core/AudioClip.h"


namespace bae
{
    class SdlAudioClip : public AudioClip
    {
    public:
        SdlAudioClip(const std::string& path, int soudId);
        ~SdlAudioClip(); // empty, needed for Pimpl

        virtual void Play() override;
        virtual void Stop() override;

        virtual void Resume() override;
        virtual void Pause() override;

        virtual void Mute() override;
        virtual void UnMute() override;


        virtual bool IsPlaying() override;
        virtual bool IsLoaded() override;

        virtual bool IsPaused() override;
        virtual bool IsMuted() override;


        virtual float GetVolume() override;
        virtual void SetVolume(float volume) override;

        virtual int GetSoundId() override;


    private:
        class Impl;
        std::unique_ptr<Impl> m_Pimpl;


    };
}


