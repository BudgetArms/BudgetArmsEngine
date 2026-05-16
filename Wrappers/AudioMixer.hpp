#pragma once

#include <SDL3_mixer/SDL_mixer.h>


namespace bae
{
    /**
     * Simple RAII wrapper for a MIX_Mixer
     */
    class AudioMixer final
    {
    public:
        explicit AudioMixer(SDL_AudioDeviceID deviceId, const SDL_AudioSpec* spec);
        ~AudioMixer();

        AudioMixer(const AudioMixer&)             = delete;
        AudioMixer(AudioMixer&&)                  = delete;
        AudioMixer& operator=(const AudioMixer&)  = delete;
        AudioMixer& operator=(const AudioMixer&&) = delete;


        [[nodiscard]] MIX_Mixer* GetMixer() const;

    private:
        MIX_Mixer* m_Mixer{};
    };
}

