#pragma once

#include <memory>

#include "Sounds/SoundSystem.h"


namespace bae
{
    class AudioTrack;

    class SdlSoundSystem final : public SoundSystem
    {
    public:
        SdlSoundSystem();
        ~SdlSoundSystem() override;


        [[nodiscard]] SoundID LoadSound(const std::string& path) override;

        ActiveSoundID Play(SoundID soundId, float volume) override;

        void Stop(ActiveSoundID activeSoundId) override;
        void Resume(ActiveSoundID activeSoundId) override;
        void Pause(ActiveSoundID activeSoundId) override;
        void Mute(ActiveSoundID activeSoundId) override;
        void UnMute(ActiveSoundID activeSoundId) override;

        [[nodiscard]] bool IsLoaded(SoundID soundId) override;
        [[nodiscard]] bool IsPlaying(ActiveSoundID activeSoundId) override;
        [[nodiscard]] bool IsPaused(ActiveSoundID activeSoundId) override;
        [[nodiscard]] bool IsMuted(ActiveSoundID activeSoundId) override;
        [[nodiscard]] float GetVolume(ActiveSoundID activeSoundId) override;
        void SetVolume(ActiveSoundID activeSoundId, float volume) override;


        void ResumeAllSounds() override;
        void PauseAllSounds() override;
        void StopAllSounds() override;
        void MuteAllSounds() override;
        void UnMuteAllSounds() override;
        void SetVolumeAllSounds(float volume) override;

        [[nodiscard]] Audio* GetAudio(SoundID soundId) override;
        [[nodiscard]] MIX_Mixer* GetMixer() override;

    private:
        // Pimple implementation
        class Impl;
        std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };
    };
}

