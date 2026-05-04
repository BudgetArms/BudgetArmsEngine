#include "AudioMixer.h"

#include <filesystem>
#include <stdexcept>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.h"


bae::AudioMixer::AudioMixer(const SDL_AudioDeviceID deviceId, const SDL_AudioSpec* spec)
{
    m_Mixer = MIX_CreateMixerDevice(deviceId, spec);

    if(!m_Mixer)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to create Mixer, Error: ") + SDL_GetError());
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

