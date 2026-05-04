#pragma once

#include <iostream>
#include <memory>

#include "Core/HelperFunctions.h"
#include "Sounds/AudioClip.h"


namespace bae
{
    template<typename RealAudioClipType,
             typename = std::enable_if_t<std::is_base_of_v<AudioClip, RealAudioClipType> &&
                 !std::is_same_v<AudioClip, RealAudioClipType>>>
    class LoggingAudioClip : public AudioClip
    {
    public:
        LoggingAudioClip(ActiveSoundID activeSoundId, SoundID soundId) :
            AudioClip(activeSoundId, soundId),
            m_RealAudioClip{}
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip = std::make_unique<RealAudioClipType>(activeSoundId, soundId);
        };

        ~LoggingAudioClip() override
        {
            std::cout << FUNCTION_NAME << '\n';
        };


        bool Play() override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->Play();
        };

        void Stop() override
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip->Stop();
        };

        void Resume() override
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip->Resume();
        };

        void Pause() override
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip->Pause();
        };

        void Mute() override
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip->Mute();
        };

        void UnMute() override
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip->UnMute();
        };


        bool IsPlaying() const override
        {
            // This is disabled because it's called constantly
            //std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->IsPlaying();
        };

        bool IsPaused() const override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->IsPaused();
        };

        bool IsMuted() const override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->IsMuted();
        };


        float GetVolume() const override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->GetVolume();
        };

        void SetVolume(float volume) override
        {
            std::cout << FUNCTION_NAME << '\n';
            m_RealAudioClip->SetVolume(volume);
        };


        SoundID GetSoundId() override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->GetSoundId();
        };

        ActiveSoundID GetActiveSoundId() override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->GetActiveSoundId();
        };

        AudioTrack& GetAudioTrack() override
        {
            std::cout << FUNCTION_NAME << '\n';
            return m_RealAudioClip->GetAudioTrack();
        };

    private:
        std::unique_ptr<RealAudioClipType> m_RealAudioClip;
    };
}

