#pragma once

#include "Sounds/SoundSystem.h"


namespace bae
{
    class SoundSystem;

    class NullSoundSystem final : public SoundSystem
    {
    public:
        NullSoundSystem()           = default;
        ~NullSoundSystem() override = default;


        [[nodiscard]] SoundID LoadSound(const std::string&) override { return SoundID{}; }

        ActiveSoundID Play(SoundID) override { return ActiveSoundID(-1); }

        void Stop(ActiveSoundID) override {}
        void Resume(ActiveSoundID) override {}
        void Pause(ActiveSoundID) override {}
        void Mute(ActiveSoundID) override {}
        void UnMute(ActiveSoundID) override {}
        void Loop(ActiveSoundID) override {}
        void UnLoop(ActiveSoundID) override {}

        void StopSounds(SoundID) override {}
        void ResumeSounds(SoundID) override {}
        void PauseSounds(SoundID) override {}
        void MuteSounds(SoundID) override {}
        void UnMuteSounds(SoundID) override {}

        void ResumeAllSounds() override {}
        void PauseAllSounds() override {}
        void StopAllSounds() override {}
        void MuteAllSounds() override {}
        void UnMuteAllSounds() override {}
        void SetVolumeAllSounds(float) override {}


        [[nodiscard]] bool IsLoaded(SoundID) override { return false; }
        [[nodiscard]] bool IsPlaying(ActiveSoundID) override { return false; }
        [[nodiscard]] bool IsPaused(ActiveSoundID) override { return false; }
        [[nodiscard]] bool IsMuted(ActiveSoundID) override { return false; }
        [[nodiscard]] float GetVolume(ActiveSoundID) override { return 0.f; }
        void SetVolume(ActiveSoundID, float) override {}


        [[nodiscard]] Audio* GetAudio(SoundID) override { return nullptr; }
        [[nodiscard]] MIX_Mixer* GetMixer() override { return nullptr; }
    };
}

