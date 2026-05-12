#pragma once
#include <string>

#include <SDL3_mixer/SDL_mixer.h>


namespace bae
{
    /**
     * Simple RAII wrapper for a MIX_Audio
     */
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

