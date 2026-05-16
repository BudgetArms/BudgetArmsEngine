#pragma once

#include <string>

#include "Sounds/SoundStructs.hpp"


namespace bae
{
    class SoundSystem
    {
    public:
        SoundSystem()          = default;
        virtual ~SoundSystem() = default;


        [[nodiscard]] virtual SoundID LoadSound(const std::string& path) = 0;

        [[nodiscard]] virtual ActiveSoundID Play(SoundID soundId) = 0;

        virtual void Stop(ActiveSoundID activeSoundId) = 0;
        virtual void Resume(ActiveSoundID activeSoundId) = 0;
        virtual void Pause(ActiveSoundID activeSoundId) = 0;
        virtual void Mute(ActiveSoundID activeSoundId) = 0;
        virtual void UnMute(ActiveSoundID activeSoundId) = 0;
        virtual void Loop(ActiveSoundID activeSoundId) = 0;
        virtual void UnLoop(ActiveSoundID activeSoundId) = 0;

        virtual void StopSounds(SoundID soundId) = 0;
        virtual void ResumeSounds(SoundID soundId) = 0;
        virtual void PauseSounds(SoundID soundId) = 0;
        virtual void MuteSounds(SoundID soundId) = 0;
        virtual void UnMuteSounds(SoundID soundId) = 0;

        // there is no PlayAllSounds bc that would be insane with 100+ loaded sounds
        virtual void StopAllSounds() = 0;
        virtual void ResumeAllSounds() = 0;
        virtual void PauseAllSounds() = 0;
        virtual void MuteAllSounds() = 0;
        virtual void UnMuteAllSounds() = 0;
        virtual void SetVolumeAllSounds(float volume) = 0;


        [[nodiscard]] virtual bool IsLoaded(SoundID soundId) = 0;
        [[nodiscard]] virtual bool IsPlaying(ActiveSoundID activeSoundId) = 0;
        [[nodiscard]] virtual bool IsPaused(ActiveSoundID activeSoundId) = 0;
        [[nodiscard]] virtual bool IsMuted(ActiveSoundID activeSoundId) = 0;
        [[nodiscard]] virtual float GetVolume(ActiveSoundID activeSoundId) = 0;
        virtual void SetVolume(ActiveSoundID activeSoundId, float volume) = 0;
    };
}

