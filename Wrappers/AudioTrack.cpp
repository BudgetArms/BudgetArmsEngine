#include "AudioTrack.hpp"

#include <filesystem>
#include <stdexcept>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"


bae::AudioTrack::AudioTrack(MIX_Mixer* mixer)
{
    m_Track = MIX_CreateTrack(mixer);
    if(!m_Track)
    {
        const std::string errorMessage = "Failed to create track, Error: " + std::string(SDL_GetError());
        Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
    }
}

bae::AudioTrack::~AudioTrack()
{
    MIX_DestroyTrack(m_Track);
}

MIX_Track* bae::AudioTrack::GetTrack() const
{
    return m_Track;
}
