#include "Audio.hpp"

#include <filesystem>
#include <stdexcept>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"


bae::Audio::Audio(const std::string& fullPath, MIX_Mixer* mixer)
{
    if(!std::filesystem::exists(fullPath))
    {
        const std::string errorMessage = "File Not Found, Path: " + std::filesystem::path(fullPath).
                string();
        Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
    }


    m_Audio = MIX_LoadAudio(mixer, fullPath.c_str(), false);
    if(!m_Audio)
    {
        const std::string errorMessage = "Failed to load audio, Error: " + std::string(SDL_GetError());
        Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
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
