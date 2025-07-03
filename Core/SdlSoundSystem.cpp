#include "SdlSoundSystem.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <iterator>
#include <map>
#include <unordered_map>

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

    void Resume(int soundId);
    void Pause(int soundId);

    void Mute(int soundId);
    void UnMute(int soundId);

    bool IsLoaded(int soundId);
    bool IsPlaying(int soundId);

    bool IsPaused(int soundId);
    bool IsMuted(int soundId);


    void ResumeAllSounds();
    void PauseAllSounds();
    void StopAllSounds();
    void MuteAllSounds();
    void UnMuteAllSounds();


private:
    std::unordered_map<std::string, std::unique_ptr<SdlAudioClip>> m_LoadedAudioClips;


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


void SdlSoundSystem::Resume(int soundId)
{
    m_Pimpl->Resume(soundId);
}

void SdlSoundSystem::Pause(int soundId)
{
    m_Pimpl->Pause(soundId);
}


void SdlSoundSystem::Mute(int soundId)
{
    m_Pimpl->Mute(soundId);
}

void SdlSoundSystem::UnMute(int soundId)
{
    m_Pimpl->UnMute(soundId);
}


bool SdlSoundSystem::IsLoaded(int soundId)
{
    return m_Pimpl->IsLoaded(soundId);
}

bool SdlSoundSystem::IsPlaying(int soundId)
{
    return m_Pimpl->IsPlaying(soundId);
}


bool SdlSoundSystem::IsPaused(int soundId)
{
    return m_Pimpl->IsPaused(soundId);
}

bool SdlSoundSystem::IsMuted(int soundId)
{
    return m_Pimpl->IsMuted(soundId);
}


void SdlSoundSystem::ResumeAllSounds()
{
    m_Pimpl->ResumeAllSounds();
}

void SdlSoundSystem::PauseAllSounds()
{
    m_Pimpl->PauseAllSounds();
}

void SdlSoundSystem::StopAllSounds()
{
    m_Pimpl->StopAllSounds();
}

void SdlSoundSystem::MuteAllSounds()
{
    m_Pimpl->MuteAllSounds();
}

void SdlSoundSystem::UnMuteAllSounds()
{
    m_Pimpl->UnMuteAllSounds();
}



#pragma endregion


#pragma region SdlSoundSystem | PIMPL


int SdlSoundSystem::Impl::LoadSound(const std::string& path)
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "SdlSoundSystem::LoadSound File " << path << " not found" << '\n';
        return -1;
    }

    // if already loaded
    if (auto it = m_LoadedAudioClips.find(path); it != m_LoadedAudioClips.end())
    {
        //int index = static_cast<int>(std::distance(m_LoadedAudioClips.begin(), it));
        int index = it->second->GetSoundId();
        return index;
    }

    int index = static_cast<int>(m_LoadedAudioClips.size());
    m_LoadedAudioClips.insert(std::pair(path, std::make_unique<SdlAudioClip>(path, index)));
    return index;
}


void SdlSoundSystem::Impl::Play(int soundId, float volume)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To" << "Play" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return;

    audioClip->SetVolume(volume);
    audioClip->Play();
}

void SdlSoundSystem::Impl::Stop(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "Stop" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return;

    audioClip->Stop();
}


void SdlSoundSystem::Impl::Resume(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "Resume" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return;

    audioClip->Resume();
}

void SdlSoundSystem::Impl::Pause(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "Pause" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return;

    audioClip->Pause();
}


void SdlSoundSystem::Impl::Mute(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "Mute" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return;

    audioClip->Mute();
}

void SdlSoundSystem::Impl::UnMute(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "UnMute" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return;

    audioClip->UnMute();
}


bool SdlSoundSystem::Impl::IsLoaded(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "IsLoaded" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return false;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return false;

    return audioClip->IsLoaded();
}

bool SdlSoundSystem::Impl::IsPlaying(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "IsPlaying" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return false;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return false;

    return audioClip->IsPlaying();
}


bool SdlSoundSystem::Impl::IsPaused(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "IsPaused" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return false;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return false;

    return audioClip->IsPaused();
}

bool SdlSoundSystem::Impl::IsMuted(int soundId)
{
    if (m_LoadedAudioClips.empty() || (soundId < 0) || (soundId >= m_LoadedAudioClips.size()))
    {
        std::cout << "Trying To " << "IsMuted" << " Sound that doesn't Exist, soundId: " << soundId << '\n';
        return false;
    }

    auto it = m_LoadedAudioClips.begin();
    std::advance(it, soundId);
    auto& audioClip = it->second;
    if (!audioClip)
        return false;

    return audioClip->IsMuted();
}


void SdlSoundSystem::Impl::ResumeAllSounds()
{
    if (m_LoadedAudioClips.empty())
    {
        std::cout << "Trying To " << "ResumeAllSounds" << " but there are no sounds to Resume" << '\n';
        return;
    }

    for (auto& [string, uAudioClip] : m_LoadedAudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->Resume();
    }

}

void SdlSoundSystem::Impl::PauseAllSounds()
{
    if (m_LoadedAudioClips.empty())
    {
        std::cout << "Trying To " << "PauseAllSounds" << " but there are no sounds to Pause" << '\n';
        return;
    }

    for (auto& [string, uAudioClip] : m_LoadedAudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->Pause();
    }

}

void SdlSoundSystem::Impl::StopAllSounds()
{
    if (m_LoadedAudioClips.empty())
    {
        std::cout << "Trying To " << "StopAllSounds" << " but there are no sounds to Stop" << '\n';
        return;
    }

    for (auto& [string, uAudioClip] : m_LoadedAudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->Stop();
    }

}

void SdlSoundSystem::Impl::MuteAllSounds()
{
    if (m_LoadedAudioClips.empty())
    {
        std::cout << "Trying To " << "MuteAllSounds" << " but there are no sounds to Mute" << '\n';
        return;
    }

    for (auto& [string, uAudioClip] : m_LoadedAudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->Mute();
    }

}

void SdlSoundSystem::Impl::UnMuteAllSounds()
{
    if (m_LoadedAudioClips.empty())
    {
        std::cout << "Trying To " << "UnMuteAllSounds" << " but there are no sounds to UnMute" << '\n';
        return;
    }

    for (auto& [string, uAudioClip] : m_LoadedAudioClips)
    {
        auto* audioClip = uAudioClip.get();
        if (audioClip)
            audioClip->UnMute();
    }

}


#pragma endregion


