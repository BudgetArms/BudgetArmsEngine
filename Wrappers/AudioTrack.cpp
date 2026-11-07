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
        Logger::LogFatal("Failed to create track, Error: " + std::string(SDL_GetError()));
        std::terminate();
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
