#pragma once

#include "Sounds/AudioClip.h"


namespace bae
{
    class NullAudioClip final : public AudioClip
    {
    public:
        explicit NullAudioClip(const ActiveSoundID activeSoundId = ActiveSoundID{ -1 },
                               const SoundID soundId             = SoundID{ -1 }) :
            AudioClip(activeSoundId, soundId)
        {
        }

        bool Play() override { return true; }
        void Stop() override {}
        void Resume() override {}
        void Pause() override {}
        void Mute() override {}
        void UnMute() override {}

        [[nodiscard]] bool IsPlaying() const override { return false; }
        [[nodiscard]] bool IsPaused() const override { return false; }
        [[nodiscard]] bool IsMuted() const override { return false; }
        [[nodiscard]] float GetVolume() const override { return 0.f; }
        void SetVolume(float) override {}

        SoundID GetSoundId() override { return SoundID(-1); }
        ActiveSoundID GetActiveSoundId() override { return ActiveSoundID(-1); }
        AudioTrack& GetAudioTrack() override { return m_AudioTrack; }

    private:
        AudioTrack m_AudioTrack{};
    };
}


