#pragma once

#include <string>

#include "Sounds/SoundStructs.h"
#include "Wrappers/Audio.h"


namespace bae
{
    class SoundSystem
    {
    public:
        SoundSystem()          = default;
        virtual ~SoundSystem() = default;


        [[nodiscard]] virtual SoundID LoadSound(const std::string& path) = 0;

        virtual ActiveSoundID Play(SoundID soundId, float volume) = 0;

        virtual void Stop(ActiveSoundID activeSoundId) = 0;
        virtual void Resume(ActiveSoundID activeSoundId) = 0;
        virtual void Pause(ActiveSoundID activeSoundId) = 0;
        virtual void Mute(ActiveSoundID activeSoundId) = 0;
        virtual void UnMute(ActiveSoundID activeSoundId) = 0;

        [[nodiscard]] virtual bool IsLoaded(SoundID soundId) = 0;
        [[nodiscard]] virtual bool IsPlaying(ActiveSoundID activeSoundId) = 0;
        [[nodiscard]] virtual bool IsPaused(ActiveSoundID activeSoundId) = 0;
        [[nodiscard]] virtual bool IsMuted(ActiveSoundID activeSoundId) = 0;
        [[nodiscard]] virtual float GetVolume(ActiveSoundID activeSoundId) = 0;
        virtual void SetVolume(ActiveSoundID activeSoundId, float volume) = 0;


        // there is no PlayAllSounds bc that would be insane with 100+ loaded sounds
        virtual void ResumeAllSounds() = 0;
        virtual void PauseAllSounds() = 0;
        virtual void StopAllSounds() = 0;
        virtual void MuteAllSounds() = 0;
        virtual void UnMuteAllSounds() = 0;
        virtual void SetVolumeAllSounds(float volume) = 0;

        [[nodiscard]] virtual Audio* GetAudio(SoundID soundId) = 0;
        [[nodiscard]] virtual MIX_Mixer* GetMixer() = 0;
    };
}

