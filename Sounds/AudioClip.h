#pragma once

#include "Sounds/SoundSystem.h"
#include "Wrappers/AudioTrack.h"


namespace bae
{
    class AudioClip
    {
    public:
        AudioClip(ActiveSoundID, SoundID) {}
        virtual ~AudioClip() = default;

        // return indicates success creation
        virtual bool Play() = 0;
        virtual void Stop() = 0;

        virtual void Resume() = 0;
        virtual void Pause() = 0;

        virtual void Mute() = 0;
        virtual void UnMute() = 0;


        [[nodiscard]] virtual bool IsPlaying() const = 0;
        [[nodiscard]] virtual bool IsPaused() const = 0;
        [[nodiscard]] virtual bool IsMuted() const = 0;

        [[nodiscard]] virtual float GetVolume() const = 0;
        virtual void SetVolume(float volume) = 0;

        [[nodiscard]] virtual SoundID GetSoundId() = 0;
        [[nodiscard]] virtual ActiveSoundID GetActiveSoundId() = 0;
        [[nodiscard]] virtual AudioTrack& GetAudioTrack() = 0;
    };
}


