#include "AudioTrack.h"

#include <filesystem>
#include <stdexcept>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"


bae::AudioTrack::AudioTrack()
{
    MIX_Mixer* const mixer = ServiceLocator::GetSoundSystem().GetMixer();

    m_Track = MIX_CreateTrack(mixer);
    if(!m_Track)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to create track, Error: ") + SDL_GetError());
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

