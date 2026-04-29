#include "Audio.h"

#include <filesystem>
#include <stdexcept>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"


bae::Audio::Audio(const std::string& fullPath)
{
    if(!std::filesystem::exists(fullPath))
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to find audio file"));
    }

    MIX_Mixer* const mixer = ServiceLocator::GetSoundSystem().GetMixer();
    m_Audio                = MIX_LoadAudio(mixer, fullPath.c_str(), false);
    if(!m_Audio)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to load audio, Error: ") + SDL_GetError());
    }
}

bae::Audio::~Audio()
{
    MIX_DestroyAudio(m_Audio);
}

MIX_Audio* bae::Audio::GetAudio() const
{
    return m_Audio;
}

