#include "SoLoudSoundSystem.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <optional>

#include <soloud.h>
#include <soloud_wav.h>

#include "Managers/ResourceManager.h"


using namespace bae;


class SoLoudSoundSystem::Impl
{
public:
    Impl();
    ~Impl();


    [[nodiscard]] SoundID LoadSound(const std::string& path);

    [[nodiscard]] ActiveSoundID Play(SoundID soundId);


    void Stop(ActiveSoundID activeSoundId);
    void Resume(ActiveSoundID activeSoundId);
    void Pause(ActiveSoundID activeSoundId);
    void Mute(ActiveSoundID activeSoundId);
    void UnMute(ActiveSoundID activeSoundId);
    void Loop(ActiveSoundID activeSoundId);
    void UnLoop(ActiveSoundID activeSoundId);


    void StopSounds(SoundID soundId);
    void ResumeSounds(SoundID soundId);
    void PauseSounds(SoundID soundId);
    void MuteSounds(SoundID soundId);
    void UnMuteSounds(SoundID soundId);


    [[nodiscard]] bool IsLoaded(SoundID soundId) const;
    [[nodiscard]] bool IsPlaying(ActiveSoundID activeSoundId);
    [[nodiscard]] bool IsPaused(ActiveSoundID activeSoundId);
    [[nodiscard]] bool IsMuted(ActiveSoundID activeSoundId);
    [[nodiscard]] float GetVolume(ActiveSoundID activeSoundId);
    void SetVolume(ActiveSoundID activeSoundId, float volume);


    void ResumeAllSounds() const;
    void PauseAllSounds() const;
    void StopAllSounds() const;
    void MuteAllSounds() const;
    void UnMuteAllSounds() const;

    void SetVolumeAllSounds(float volume) const;

private:
    std::optional<SoLoud::handle> GetHandle(ActiveSoundID activeSoundId);
    std::optional<SoLoud::Wav*> GetSoundsWav(SoundID soundId);

    std::unordered_map<std::string, SoundID> m_LoadedSoundIDs{};
    std::unordered_map<SoundID, std::unique_ptr<SoLoud::Wav>> m_LoadedAudio{};
    std::unordered_map<ActiveSoundID, SoLoud::handle> m_ActiveAudio;

    int m_SoundIdValue{};

    std::unique_ptr<SoLoud::Soloud> m_SoLoud{};
};


#pragma region NOT PIMPL


SoLoudSoundSystem::SoLoudSoundSystem()  = default;
SoLoudSoundSystem::~SoLoudSoundSystem() = default;


SoundID SoLoudSoundSystem::LoadSound(const std::string& path)
{
    return m_Pimpl->LoadSound(path);
}


ActiveSoundID SoLoudSoundSystem::Play(const SoundID soundId)
{
    return m_Pimpl->Play(soundId);
}

void SoLoudSoundSystem::Stop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Stop(activeSoundId);
}


void SoLoudSoundSystem::Resume(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Resume(activeSoundId);
}

void SoLoudSoundSystem::Pause(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Pause(activeSoundId);
}


void SoLoudSoundSystem::Mute(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Mute(activeSoundId);
}

void SoLoudSoundSystem::UnMute(const ActiveSoundID activeSoundId)
{
    m_Pimpl->UnMute(activeSoundId);
}

void SoLoudSoundSystem::Loop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Loop(activeSoundId);
}

void SoLoudSoundSystem::UnLoop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->UnLoop(activeSoundId);
}


void SoLoudSoundSystem::StopSounds(const SoundID soundId)
{
    m_Pimpl->StopSounds(soundId);
}

void SoLoudSoundSystem::ResumeSounds(const SoundID soundId)
{
    m_Pimpl->ResumeSounds(soundId);
}

void SoLoudSoundSystem::PauseSounds(const SoundID soundId)
{
    m_Pimpl->PauseSounds(soundId);
}

void SoLoudSoundSystem::MuteSounds(const SoundID soundId)
{
    m_Pimpl->MuteSounds(soundId);
}

void SoLoudSoundSystem::UnMuteSounds(const SoundID soundId)
{
    m_Pimpl->UnMuteSounds(soundId);
}


void SoLoudSoundSystem::ResumeAllSounds()
{
    m_Pimpl->ResumeAllSounds();
}

void SoLoudSoundSystem::PauseAllSounds()
{
    m_Pimpl->PauseAllSounds();
}

void SoLoudSoundSystem::StopAllSounds()
{
    m_Pimpl->StopAllSounds();
}

void SoLoudSoundSystem::MuteAllSounds()
{
    m_Pimpl->MuteAllSounds();
}

void SoLoudSoundSystem::UnMuteAllSounds()
{
    m_Pimpl->UnMuteAllSounds();
}

void SoLoudSoundSystem::SetVolumeAllSounds(const float volume)
{
    m_Pimpl->SetVolumeAllSounds(volume);
}


bool SoLoudSoundSystem::IsLoaded(const SoundID soundId)
{
    return m_Pimpl->IsLoaded(soundId);
}

bool SoLoudSoundSystem::IsPlaying(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsPlaying(activeSoundId);
}


bool SoLoudSoundSystem::IsPaused(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsPaused(activeSoundId);
}

bool SoLoudSoundSystem::IsMuted(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsMuted(activeSoundId);
}


float SoLoudSoundSystem::GetVolume(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->GetVolume(activeSoundId);
}

void SoLoudSoundSystem::SetVolume(const ActiveSoundID activeSoundId, const float volume)
{
    m_Pimpl->SetVolume(activeSoundId, volume);
}


#pragma endregion


#pragma region PIMPL


SoLoudSoundSystem::Impl::Impl()
{
    m_SoLoud = std::make_unique<SoLoud::Soloud>();
    m_SoLoud->init();
}

SoLoudSoundSystem::Impl::~Impl()
{
    // Stop all Playing sounds
    std::ranges::for_each(m_ActiveAudio,
                          [&](auto& activeAudio)
                          {
                              const auto& [activeSoundID, soundHandle] = activeAudio;
                              m_SoLoud->stop(soundHandle);
                          });

    m_ActiveAudio.clear();


    // Stop all loaded audio
    std::ranges::for_each(m_LoadedAudio,
                          [&](auto& loadedAudio)
                          {
                              SoLoud::Wav* wav = loadedAudio.second.get();
                              wav->stop();
                          });

    m_LoadedAudio.clear();

    m_SoLoud->deinit();
}


SoundID SoLoudSoundSystem::Impl::LoadSound(const std::string& path)
{
    // Already loaded
    if(const auto it = m_LoadedSoundIDs.find(path); it != m_LoadedSoundIDs.end())
    {
        return it->second;
    }

    const std::filesystem::path soundPath = ResourceManager::GetInstance().GetResourcesPath() / path.c_str();

    auto loadedAudio            = std::make_unique<SoLoud::Wav>();
    const SoLoud::result result = loadedAudio->load(soundPath.string().c_str());

    if(result != SoLoud::SOLOUD_ERRORS::SO_NO_ERROR)
    {
        std::cout << FUNCTION_NAME << " Failed to load SoLoud::Wav: " << result << '\n';
        return SoundID{ -1 };
    }


    const SoundID soundId{ m_SoundIdValue };
    ++m_SoundIdValue;

    m_LoadedAudio.insert({ soundId, std::move(loadedAudio) });

    return soundId;
}


ActiveSoundID SoLoudSoundSystem::Impl::Play(const SoundID soundId)
{
    if(!IsLoaded(soundId))
    {
        std::cout << FUNCTION_NAME << " Failed! No Sound loaded with that SoundID: " << soundId.ID << '\n';
        return ActiveSoundID{ -1 };
    }

    SoLoud::Wav* const uSound = m_LoadedAudio.at(soundId).get();
    if(!uSound)
    {
        std::cout << FUNCTION_NAME << " Failed! Audio with that SoundID is invalid, SoundID: " << soundId.ID << '\n';
        return ActiveSoundID{ -1 };
    }

    const SoLoud::handle handleSound = m_SoLoud->play(*uSound);
    if(handleSound == SoLoud::SOLOUD_ERRORS::UNKNOWN_ERROR)
    {
        std::cout << FUNCTION_NAME << " Failed! Audio couldn't play for unknown reason, SoundID: " << soundId.ID
                << '\n';
        return ActiveSoundID{ -1 };
    }

    const ActiveSoundID activeSoundId{ ActiveSoundID::GenerateActiveSoundID() };
    m_ActiveAudio.insert({ activeSoundId, handleSound });

    return activeSoundId;
}

void SoLoudSoundSystem::Impl::Stop(const ActiveSoundID activeSoundId)
{
    std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->stop(optHandle.value());
}


void SoLoudSoundSystem::Impl::Resume(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->setPause(optHandle.value(), false);
}

void SoLoudSoundSystem::Impl::Pause(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->setPause(optHandle.value(), true);
}


void SoLoudSoundSystem::Impl::Mute(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->setVolume(optHandle.value(), 0.0f);
}

void SoLoudSoundSystem::Impl::UnMute(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->setVolume(optHandle.value(), 1.0f);
}


void SoLoudSoundSystem::Impl::Loop(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->setLooping(optHandle.value(), true);
}

void SoLoudSoundSystem::Impl::UnLoop(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    m_SoLoud->setLooping(optHandle.value(), false);
}


void SoLoudSoundSystem::Impl::StopSounds(const SoundID soundId)
{
    const std::optional<SoLoud::Wav*> optSound = GetSoundsWav(soundId);
    if(!optSound.has_value())
    {
        return;
    }

    optSound.value()->stop();
}

void SoLoudSoundSystem::Impl::ResumeSounds(const SoundID soundId)
{
    const std::optional<SoLoud::Wav*> optSound = GetSoundsWav(soundId);
    if(!optSound.has_value())
    {
        return;
    }
}


void SoLoudSoundSystem::Impl::PauseSounds(const SoundID soundId)
{
    const std::optional<SoLoud::Wav*> optSound = GetSoundsWav(soundId);
    if(!optSound.has_value())
    {
        return;
    }
}


void SoLoudSoundSystem::Impl::MuteSounds(const SoundID soundId)
{
    const std::optional<SoLoud::Wav*> optSound = GetSoundsWav(soundId);
    if(!optSound.has_value())
    {
        return;
    }

    optSound.value()->setVolume(0.0f);
}

void SoLoudSoundSystem::Impl::UnMuteSounds(const SoundID soundId)
{
    const std::optional<SoLoud::Wav*> optSound = GetSoundsWav(soundId);
    if(!optSound.has_value())
    {
        return;
    }

    optSound.value()->setVolume(1.f);
}

bool SoLoudSoundSystem::Impl::IsLoaded(const SoundID soundId) const
{
    return m_LoadedAudio.contains(soundId);
}

bool SoLoudSoundSystem::Impl::IsPlaying(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return false;
    }

    return m_SoLoud->isValidVoiceHandle(optHandle.value());
}

bool SoLoudSoundSystem::Impl::IsPaused(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return false;
    }

    return m_SoLoud->getPause(optHandle.value());
}

bool SoLoudSoundSystem::Impl::IsMuted(const ActiveSoundID activeSoundId)
{
    return GetVolume(activeSoundId) == 0.0f;
}

float SoLoudSoundSystem::Impl::GetVolume(const ActiveSoundID activeSoundId)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return 0.f;
    }

    return m_SoLoud->getVolume(optHandle.value());
}

void SoLoudSoundSystem::Impl::SetVolume(const ActiveSoundID activeSoundId, const float volume)
{
    const std::optional<SoLoud::handle> optHandle = GetHandle(activeSoundId);
    if(!optHandle.has_value())
    {
        return;
    }

    return m_SoLoud->setVolume(optHandle.value(), volume);
}


void SoLoudSoundSystem::Impl::ResumeAllSounds() const
{
    m_SoLoud->setPauseAll(false);
}

void SoLoudSoundSystem::Impl::PauseAllSounds() const
{
    m_SoLoud->setPauseAll(true);
}

void SoLoudSoundSystem::Impl::StopAllSounds() const
{
    m_SoLoud->stopAll();
}

void SoLoudSoundSystem::Impl::MuteAllSounds() const
{
    m_SoLoud->setGlobalVolume(0.f);
}

void SoLoudSoundSystem::Impl::UnMuteAllSounds() const
{
    m_SoLoud->setGlobalVolume(1.f);
}

void SoLoudSoundSystem::Impl::SetVolumeAllSounds(const float volume) const
{
    m_SoLoud->setGlobalVolume(volume);
}

std::optional<SoLoud::handle> SoLoudSoundSystem::Impl::GetHandle(const ActiveSoundID activeSoundId)
{
    const auto it = m_ActiveAudio.find(activeSoundId);
    if(it == m_ActiveAudio.end())
    {
        return std::nullopt;
    }

    return it->second;
}

std::optional<SoLoud::Wav*> SoLoudSoundSystem::Impl::GetSoundsWav(const SoundID soundId)
{
    if(!IsLoaded(soundId))
    {
        return std::nullopt;
    }

    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        return std::nullopt;
    }

    return it->second.get();
}


#pragma endregion
