#include "SdlSoundSystem.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include "Core/AudioClip.h"
#include "Core/SdlAudioClip.h"

#include <SDL_mixer.h>


using namespace bae;


class SdlSoundSystem::Impl
{
public:
    int LoadSound(const std::string& path);

    void Play(int soundId, float volume);
    void Stop(int soundId);
    void Mute(int soundId);

    bool IsLoaded(int soundId);
    bool IsPlaying(int soundId);


    void StopAllSounds();
    void MuteAllSounds();


private:
    std::vector<std::unique_ptr<SdlAudioClip>> m_AudioClips;


};



#pragma region SdlSoundSystem | NOT PIMPL


SdlSoundSystem::SdlSoundSystem()
{
}

SdlSoundSystem::~SdlSoundSystem()
{
}


int SdlSoundSystem::LoadSound(const std::string& path)
{
    return m_Pimpl->LoadSound(path);
}


void SdlSoundSystem::Play(int soundId, float volume)
{
    m_Pimpl->Play(soundId, volume);
}

void SdlSoundSystem::Stop(int soundId)
{
    m_Pimpl->Stop(soundId);
}

void SdlSoundSystem::Mute(int soundId)
{
    m_Pimpl->Mute(soundId);
}


bool SdlSoundSystem::IsLoaded(int soundId)
{
    return m_Pimpl->IsLoaded(soundId);
}

bool SdlSoundSystem::IsPlaying(int soundId)
{
    return m_Pimpl->IsPlaying(soundId);
}


void SdlSoundSystem::StopAllSounds()
{
    m_Pimpl->StopAllSounds();
}

void SdlSoundSystem::MuteAllSounds()
{
    m_Pimpl->MuteAllSounds();
}



#pragma endregion


#pragma region SdlSoundSystem | PIMPL


int SdlSoundSystem::Impl::LoadSound(const std::string& path)
{
    if (!std::filesystem::exists(path))
        return -1; // this does not follows Guy Davidsons advice

    m_AudioClips.emplace_back(std::make_unique<SdlAudioClip>(path));
    return static_cast<int>(m_AudioClips.size() - 1);
}


void SdlSoundSystem::Impl::Play(int soundId, float volume)
{
    if (m_AudioClips.empty() || (soundId < 0) || (soundId >= m_AudioClips.size()))
    {
        std::cout << "Trying To" << "Play" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto& audioClip = m_AudioClips[soundId];
    if (!audioClip)
        return;

    audioClip->SetVolume(volume);
    audioClip->Play();
}

void SdlSoundSystem::Impl::Stop(int soundId)
{
    if (m_AudioClips.empty() || (soundId < 0) || (soundId >= m_AudioClips.size()))
    {
        std::cout << "Trying To " << "Stop" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto& audioClip = m_AudioClips[soundId];
    if (!audioClip)
        return;

    audioClip->Stop();
}

void SdlSoundSystem::Impl::Mute(int soundId)
{
    if (m_AudioClips.empty() || (soundId < 0) || (soundId >= m_AudioClips.size()))
    {
        std::cout << "Trying To " << "Mute" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto& audioClip = m_AudioClips[soundId];
    if (!audioClip)
        return;

    audioClip->Mute();
}


bool SdlSoundSystem::Impl::IsLoaded(int soundId)
{
    if (m_AudioClips.empty() || (soundId < 0) || (soundId >= m_AudioClips.size()))
    {
        std::cout << "Trying To " << "IsLoaded" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return false;
    }

    auto& audioClip = m_AudioClips[soundId];
    if (!audioClip)
        return false;

    return audioClip->IsLoaded();
}

bool SdlSoundSystem::Impl::IsPlaying(int soundId)
{
    if (m_AudioClips.empty() || (soundId < 0) || (soundId >= m_AudioClips.size()))
    {
        std::cout << "Trying To " << "IsPlaying" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return false;
    }

    auto& audioClip = m_AudioClips[soundId];
    if (!audioClip)
        return false;

    return audioClip->IsPlaying();
}


void SdlSoundSystem::Impl::StopAllSounds()
{
    if (m_AudioClips.empty())
    {
        std::cout << "Trying To " << "StopAllSounds" << " but there are no sounds to Stop" << '\n';
        return;
    }

    for (auto& uAudioClip : m_AudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->Stop();
    }

}

void SdlSoundSystem::Impl::MuteAllSounds()
{
    if (m_AudioClips.empty())
    {
        std::cout << "Trying To " << "MuteAllSounds" << " but there are no sounds to Stop" << '\n';
        return;
    }

    for (auto& uAudioClip : m_AudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->Mute();
    }

}





#pragma endregion


