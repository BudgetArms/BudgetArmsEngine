#pragma once

#include <SDL3_mixer/SDL_mixer.h>


namespace bae
{
    /**
     * Simple RAII wrapper for a Mix_Track
     */
    // TODO: Note that this could be a abstract class with templates, so that
    // you could have AudioTrack, and you would hide that implementation with Pimpl
    // And then you could have XTrack, for another audio system
    // And then SoundSystem or more specifically SdlSoundSystem::Impl uses MIX_Track
    // it could be in SoundSystem, if MIX_Audio was not tied to Mixer
    class AudioTrack final
    {
    public:
        explicit AudioTrack();
        ~AudioTrack();

        AudioTrack(const AudioTrack&)             = delete;
        AudioTrack(AudioTrack&&)                  = delete;
        AudioTrack& operator=(const AudioTrack&)  = delete;
        AudioTrack& operator=(const AudioTrack&&) = delete;


        [[nodiscard]] MIX_Track* GetTrack() const;

    private:
        MIX_Track* m_Track{};
    };
}

