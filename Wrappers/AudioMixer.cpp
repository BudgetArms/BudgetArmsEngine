#include "AudioMixer.hpp"

#include <filesystem>
#include <stdexcept>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"


bae::AudioMixer::AudioMixer(const SDL_AudioDeviceID deviceId, const SDL_AudioSpec* spec)
{
    m_Mixer = MIX_CreateMixerDevice(deviceId, spec);

    if(!m_Mixer)
    {
        Logger::LogFatal("Failed to create Mixer, Error: " + std::string(SDL_GetError()));
        std::terminate();
    }
}

bae::AudioMixer::~AudioMixer()
{
    MIX_DestroyMixer(m_Mixer);
}

MIX_Mixer* bae::AudioMixer::GetMixer() const
{
    return m_Mixer;
}
