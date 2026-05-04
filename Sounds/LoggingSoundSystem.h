#pragma once

#include <memory>

#include "Sounds/SoundStructs.h"
#include "Sounds/SoundSystem.h"


namespace bae
{
    class LoggingSoundSystem final : public SoundSystem
    {
    public:
        explicit LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem);
        ~LoggingSoundSystem() override;

        [[nodiscard]] SoundID LoadSound(const std::string& path) override;

        [[nodiscard]] ActiveSoundID Play(SoundID soundId) override;

        void Stop(ActiveSoundID activeSoundId) override;
        void Resume(ActiveSoundID activeSoundId) override;
        void Pause(ActiveSoundID activeSoundId) override;
        void Mute(ActiveSoundID activeSoundId) override;
        void UnMute(ActiveSoundID activeSoundId) override;
        void Loop(ActiveSoundID activeSoundId) override;
        void UnLoop(ActiveSoundID activeSoundId) override;

        void StopSounds(SoundID soundId) override;
        void ResumeSounds(SoundID soundId) override;
        void PauseSounds(SoundID soundId) override;
        void MuteSounds(SoundID soundId) override;
        void UnMuteSounds(SoundID soundId) override;

        void ResumeAllSounds() override;
        void PauseAllSounds() override;
        void StopAllSounds() override;
        void MuteAllSounds() override;
        void UnMuteAllSounds() override;
        void SetVolumeAllSounds(float volume) override;


        [[nodiscard]] bool IsLoaded(SoundID soundId) override;
        [[nodiscard]] bool IsPlaying(ActiveSoundID activeSoundId) override;
        [[nodiscard]] bool IsPaused(ActiveSoundID activeSoundId) override;
        [[nodiscard]] bool IsMuted(ActiveSoundID activeSoundId) override;
        [[nodiscard]] float GetVolume(ActiveSoundID activeSoundId) override;
        void SetVolume(ActiveSoundID activeSoundId, float volume) override;


        [[nodiscard]] Audio* GetAudio(SoundID soundId) override;
        [[nodiscard]] MIX_Mixer* GetMixer() override;

    private:
        std::unique_ptr<SoundSystem> m_RealSoundSystem;
    };
}

