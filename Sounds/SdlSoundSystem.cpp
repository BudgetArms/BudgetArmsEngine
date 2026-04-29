#include "SdlSoundSystem.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"
#include "Sounds/AudioClip.h"
#include "Sounds/NullAudioClip.h"
#include "Wrappers/Audio.h"


using namespace bae;


class SdlSoundSystem::Impl
{
public:
    Impl();
    ~Impl();


    SoundID LoadSound(const std::string& path);

    ActiveSoundID Play(SoundID soundId, float volume);
    void Stop(ActiveSoundID activeSoundId) const;

    void Resume(ActiveSoundID activeSoundId) const;
    void Pause(ActiveSoundID activeSoundId) const;

    void Mute(ActiveSoundID activeSoundId) const;
    void UnMute(ActiveSoundID activeSoundId) const;

    [[nodiscard]] bool IsLoaded(SoundID soundId) const;
    [[nodiscard]] bool IsPlaying(ActiveSoundID activeSoundId) const;

    [[nodiscard]] bool IsPaused(ActiveSoundID activeSoundId) const;
    [[nodiscard]] bool IsMuted(ActiveSoundID activeSoundId) const;

    [[nodiscard]] float GetVolume(ActiveSoundID activeSoundId) const;
    void SetVolume(ActiveSoundID activeSoundId, float volume) const;


    void ResumeAllSounds() const;
    void PauseAllSounds() const;
    void StopAllSounds() const;
    void MuteAllSounds() const;
    void UnMuteAllSounds() const;

    void SetVolumeAllSounds(float volume) const;

    Audio* GetAudio(SoundID soundId);
    [[nodiscard]] MIX_Mixer* GetMixer() const;

private:
    std::unordered_map<std::string, SoundID> m_LoadedSoundIDs;
    std::unordered_map<SoundID, std::unique_ptr<Audio>> m_LoadedAudio;

    MIX_Mixer* m_Mixer{};
};


#pragma region SdlSoundSystem | NOT PIMPL


SdlSoundSystem::SdlSoundSystem()  = default;
SdlSoundSystem::~SdlSoundSystem() = default;


SoundID SdlSoundSystem::LoadSound(const std::string& path)
{
    return m_Pimpl->LoadSound(path);
}


ActiveSoundID SdlSoundSystem::Play(const SoundID soundId, const float volume)
{
    return m_Pimpl->Play(soundId, volume);
}

void SdlSoundSystem::Stop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Stop(activeSoundId);
}


void SdlSoundSystem::Resume(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Resume(activeSoundId);
}

void SdlSoundSystem::Pause(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Pause(activeSoundId);
}


void SdlSoundSystem::Mute(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Mute(activeSoundId);
}

void SdlSoundSystem::UnMute(const ActiveSoundID activeSoundId)
{
    m_Pimpl->UnMute(activeSoundId);
}


bool SdlSoundSystem::IsLoaded(const SoundID soundId)
{
    return m_Pimpl->IsLoaded(soundId);
}

bool SdlSoundSystem::IsPlaying(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsPlaying(activeSoundId);
}


bool SdlSoundSystem::IsPaused(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsPaused(activeSoundId);
}

bool SdlSoundSystem::IsMuted(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsMuted(activeSoundId);
}


float SdlSoundSystem::GetVolume(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->GetVolume(activeSoundId);
}

void SdlSoundSystem::SetVolume(const ActiveSoundID activeSoundId, const float volume)
{
    m_Pimpl->SetVolume(activeSoundId, volume);
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

void SdlSoundSystem::SetVolumeAllSounds(const float volume)
{
    m_Pimpl->SetVolumeAllSounds(volume);
}

Audio* SdlSoundSystem::GetAudio(const SoundID soundId)
{
    return m_Pimpl->GetAudio(soundId);
}

MIX_Mixer* SdlSoundSystem::GetMixer()
{
    return m_Pimpl->GetMixer();
}


#pragma endregion


#pragma region SdlSoundSystem | PIMPL


SdlSoundSystem::Impl::Impl()
{
    if(!MIX_Init())
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to initialize Mixer, Error: ") + SDL_GetError());
    }

    m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if(!m_Mixer)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to create mixer, Error: ") + SDL_GetError());
    }
}

SdlSoundSystem::Impl::~Impl()
{
    ServiceLocator::RegisterAudioQueue<NullAudioClip>();

    m_LoadedAudio.clear();

    // TODO: update
    if(SDL_WasInit(SDL_INIT_AUDIO))
    {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}


SoundID SdlSoundSystem::Impl::LoadSound(const std::string& path)
{
    if(!std::filesystem::exists(path))
    {
        std::cerr << "SdlSoundSystem::LoadSound File " << path << " not found" << '\n';
        return SoundID{};
    }

    // if already loaded
    if(const auto it = m_LoadedSoundIDs.find(path); it != m_LoadedSoundIDs.end())
    {
        return it->second;
    }

    // Set before inserting so the soundId starts from 0, and not from 1
    SoundID soundId{ .ID = static_cast<int>(m_LoadedAudio.size()) };

    // load audio chunk
    m_LoadedSoundIDs.insert(std::pair(path, soundId));
    m_LoadedAudio.insert(std::pair(soundId, std::make_unique<Audio>(path)));

    return soundId;
}


ActiveSoundID SdlSoundSystem::Impl::Play(const SoundID soundId, const float volume)
{
    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " Sound that doesn't Exist, soundId: " << soundId.ID << '\n';
        return ActiveSoundID{};
    }

    if(const auto& audioChunk = it->second; !audioChunk)
    {
        return ActiveSoundID(-1);
    }

    // this also gives an error if not in the correct initialization order :D
    const SoundEventData data
    {
        .Type          = SoundEventType::Play,
        .SoundID       = soundId,
        .ActiveSoundID = ActiveSoundID{ .ID = GetRandomNumber() },
        .Volume        = volume,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);

    return data.ActiveSoundID;
}

void SdlSoundSystem::Impl::Stop(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::Stop,
        .ActiveSoundID = activeSoundId,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


void SdlSoundSystem::Impl::Resume(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::Resume,
        .ActiveSoundID = activeSoundId,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::Pause(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::Pause,
        .ActiveSoundID = activeSoundId,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


void SdlSoundSystem::Impl::Mute(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::Mute,
        .ActiveSoundID = activeSoundId,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMute(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::UnMute,
        .ActiveSoundID = activeSoundId,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


bool SdlSoundSystem::Impl::IsLoaded(const SoundID soundId) const
{
    if(!m_LoadedAudio.contains(soundId))
    {
        return false;
    }

    return true;
}

bool SdlSoundSystem::Impl::IsPlaying(const ActiveSoundID activeSoundId) const
{
    // this is special bc we are sending request, and you can't immediately get a response back
    // OR
    // we don't use the audio queue's thread and get the m_ActiveSound's or something like that, ...
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return false;
    }

    const auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        return false;
    }

    return pAudioClip->IsPlaying();
}


bool SdlSoundSystem::Impl::IsPaused(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return false;
    }


    const auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        return false;
    }

    return pAudioClip->IsPaused();
}

bool SdlSoundSystem::Impl::IsMuted(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return false;
    }


    const auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        return false;
    }

    return pAudioClip->IsMuted();
}


float SdlSoundSystem::Impl::GetVolume(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return -1.f;
    }


    const auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        return -1.f;
    }

    return pAudioClip->GetVolume();
}

void SdlSoundSystem::Impl::SetVolume(const ActiveSoundID activeSoundId, const float volume) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type          = SoundEventType::SetVolume,
        .ActiveSoundID = activeSoundId,
        .Volume        = volume
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


void SdlSoundSystem::Impl::ResumeAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }

    constexpr SoundEventData data
    {
        .Type = SoundEventType::ResumeAll,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::PauseAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    constexpr SoundEventData data
    {
        .Type = SoundEventType::PauseAll,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::StopAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    constexpr SoundEventData data
    {
        .Type = SoundEventType::StopAll,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::MuteAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    constexpr SoundEventData data
    {
        .Type = SoundEventType::MuteAll,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMuteAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    constexpr SoundEventData data
    {
        .Type = SoundEventType::UnMuteAll,
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::SetVolumeAllSounds(const float volume) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but no sound is loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type   = SoundEventType::SetVolumeAll,
        .Volume = volume
    };

    ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

Audio* SdlSoundSystem::Impl::GetAudio(const SoundID soundId)
{
    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        std::cout << "Trying To " << FUNCTION_NAME << " but there are no sounds to UnMute" << '\n';
        return nullptr;
    }

    return it->second.get();
}

MIX_Mixer* SdlSoundSystem::Impl::GetMixer() const
{
    return m_Mixer;
}


#pragma endregion

