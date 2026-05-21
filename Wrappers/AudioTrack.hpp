#pragma once

#include <SDL3_mixer/SDL_mixer.h>


namespace bae
{
    /**
     * Simple RAII wrapper for a Mix_Track
     */
    class AudioTrack final
    {
    public:
        explicit AudioTrack(MIX_Mixer* mixer);
        ~AudioTrack();

        AudioTrack(const AudioTrack&)            = delete;
        AudioTrack(AudioTrack&&)                 = delete;
        AudioTrack& operator=(const AudioTrack&) = delete;
        AudioTrack& operator=(AudioTrack&&)      = delete;


        [[nodiscard]] MIX_Track* GetTrack() const;

    private:
        MIX_Track* m_Track{};
    };
}

