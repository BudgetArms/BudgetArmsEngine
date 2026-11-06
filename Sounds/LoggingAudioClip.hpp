#pragma once

#include <memory>

#include "Core/Logger.hpp"
#include "Sounds/AudioClip.hpp"


namespace bae
{
    template<typename RealAudioClipType,
             typename = std::enable_if_t<std::is_base_of_v<AudioClip, RealAudioClipType> &&
                 !std::is_same_v<AudioClip, RealAudioClipType>>>
    class LoggingAudioClip : public AudioClip
    {
    public:
        LoggingAudioClip(ActiveSoundID activeSoundId, SoundID soundId) :
            AudioClip(activeSoundId, soundId)
        {
            Logger::LogNormal("");
            m_RealAudioClip = std::make_unique<RealAudioClipType>(activeSoundId, soundId);
        };

        ~LoggingAudioClip() override
        {
            Logger::LogNormal("");
        };


        bool Play() override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->Play();
        };

        void Stop() override
        {
            Logger::LogNormal("");
            m_RealAudioClip->Stop();
        };

        void Resume() override
        {
            Logger::LogNormal("");
            m_RealAudioClip->Resume();
        };

        void Pause() override
        {
            Logger::LogNormal("");
            m_RealAudioClip->Pause();
        };

        void Mute() override
        {
            Logger::LogNormal("");
            m_RealAudioClip->Mute();
        };

        void UnMute() override
        {
            Logger::LogNormal("");
            m_RealAudioClip->UnMute();
        };


        // No Logs since it's called constantly
        bool IsPlaying() const override
        {
            return m_RealAudioClip->IsPlaying();
        };

        bool IsPaused() const override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->IsPaused();
        };

        bool IsMuted() const override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->IsMuted();
        };


        float GetVolume() const override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->GetVolume();
        };

        void SetVolume(float volume) override
        {
            Logger::LogNormal("");
            m_RealAudioClip->SetVolume(volume);
        };


        SoundID GetSoundId() override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->GetSoundId();
        };

        ActiveSoundID GetActiveSoundId() override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->GetActiveSoundId();
        };

        AudioTrack& GetAudioTrack() override
        {
            Logger::LogNormal("");
            return m_RealAudioClip->GetAudioTrack();
        };

    private:
        std::unique_ptr<RealAudioClipType> m_RealAudioClip{};
    };
}
