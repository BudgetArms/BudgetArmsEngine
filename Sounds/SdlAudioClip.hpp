#pragma once

#include <memory>

#include "Sounds/AudioClip.hpp"
#include "Wrappers/Audio.hpp"


namespace bae
{
    class SdlAudioClip final : public AudioClip
    {
    public:
        SdlAudioClip(ActiveSoundID activeId, SoundID soundId, MIX_Mixer* mixer, const Audio* audio);
        ~SdlAudioClip() override; // empty, needed for Pimpl

        bool Play() override;
        void Stop() override;
        void Resume() override;
        void Pause() override;
        void Mute() override;
        void UnMute() override;
        void Loop() override;
        void UnLoop() override;


        [[nodiscard]] bool IsPlaying() const override;
        [[nodiscard]] bool IsPaused() const override;
        [[nodiscard]] bool IsMuted() const override;
        [[nodiscard]] bool IsStopped() const override;


        [[nodiscard]] float GetVolume() const override;
        void SetVolume(float volume) override;

        [[nodiscard]] SoundID GetSoundId() override;
        [[nodiscard]] ActiveSoundID GetActiveSoundId() override;
        [[nodiscard]] AudioTrack& GetAudioTrack() override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_Pimpl;
    };
}


