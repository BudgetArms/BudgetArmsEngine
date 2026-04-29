#pragma once
#include <string>

#include <SDL3_mixer/SDL_mixer.h>


namespace bae
{
    /**
     * Simple RAII wrapper for a MIX_Audio
     */
    // TODO: Note that this could be a abstract class with templates, so that
    // you could have Audio, and you would hide that implementation with Pimpl
    // And then you could have XAudio, for another audio system
    // And then SoundSystem or more specifically SdlSoundSystem::Impl uses MIX_Audio
    // it could be in SoundSystem, if MIX_Audio was not tied to Mixer
    class Audio final
    {
    public:
        explicit Audio(const std::string& fullPath);
        ~Audio();

        Audio(const Audio&)             = delete;
        Audio(Audio&&)                  = delete;
        Audio& operator=(const Audio&)  = delete;
        Audio& operator=(const Audio&&) = delete;


        [[nodiscard]] MIX_Audio* GetAudio() const;

    private:
        MIX_Audio* m_Audio{};
    };
}

