#include "MixerSoundSystem.h"

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include <cassert>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ranges>
#include <stop_token>
#include <string>
#include <thread>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "SdlAudioClip.h"
#include "Core/HelperFunctions.h"
#include "Core/RingBuffer.h"
#include "Core/ServiceLocator.h"
#include "Sounds/AudioClip.h"
#include "Wrappers/Audio.h"


using namespace bae;

namespace bae
{
    class AudioQueue;
}

class MixerSoundSystem::Impl
{
public:
    Impl();
    ~Impl();


    [[nodiscard]] SoundID LoadSound(const std::string& path);

    [[nodiscard]] ActiveSoundID Play(SoundID soundId) const;


    void Stop(ActiveSoundID activeSoundId) const;
    void Resume(ActiveSoundID activeSoundId) const;
    void Pause(ActiveSoundID activeSoundId) const;
    void Mute(ActiveSoundID activeSoundId) const;
    void UnMute(ActiveSoundID activeSoundId) const;
    void Loop(ActiveSoundID activeSoundId) const;
    void UnLoop(ActiveSoundID activeSoundId) const;


    void StopSounds(SoundID soundId) const;
    void ResumeSounds(SoundID soundId) const;
    void PauseSounds(SoundID soundId) const;
    void MuteSounds(SoundID soundId) const;
    void UnMuteSounds(SoundID soundId) const;


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

private:
    std::unordered_map<std::string, SoundID> m_LoadedSoundIDs{};
    std::unique_ptr<AudioQueue> m_AudioQueue{};

    int m_SoundIdValue{};
};


namespace bae
{
    enum class SoundEventType
    {
        PlaySound,
        StopSound,
        ResumeSound,
        PauseSound,
        MuteSound,
        UnMuteSound,
        LoopSound,
        UnLoopSound,
        SetVolumeSound,
        //
        StopSounds,
        ResumeSounds,
        PauseSounds,
        MuteSounds,
        UnMuteSounds,
        //
        StopAll,
        ResumeAll,
        PauseAll,
        MuteAll,
        UnMuteAll,
        SetVolumeAll,
    };

    struct SoundEventData
    {
        SoundEventType Type{};
        SoundID SoundID{ -1 };
        ActiveSoundID ActiveSoundID{ -1 };
        float Volume{ 1.f };
    };

    class AudioQueue final
    {
    public:
        AudioQueue();
        ~AudioQueue();

        void SendSoundEvent(const SoundEventData& soundEvent);
        const AudioClip* GetAudioClip(ActiveSoundID activeSoundId);

        bool IsAudioLoaded(SoundID soundId);
        void AddAudio(SoundID soundId, const std::string& path);

    private:
        void AudioThreadLoop(const std::stop_token& stopToken);

        void ProcessSoundEvent(const SoundEventData& eventData);
        void CleanUpFinishedSounds();

        [[nodiscard]] static constexpr bool IsAudioClipNeededForSoundEventType(const SoundEventType& eventType);
        [[nodiscard]] static constexpr bool IsValidSoundIDNeededForSoundEventType(const SoundEventType& eventType);

        bool IsAudioLoadedUnLocked(SoundID soundId);

        Audio* GetAudio(SoundID soundId);

        RingBuffer<SoundEventData> m_SoundEventBuffer;
        std::unordered_map<ActiveSoundID, std::unique_ptr<SdlAudioClip>> m_ActiveAudio;

        std::jthread m_AudioThread;

        std::mutex m_MainThreadMutex{};
        std::mutex m_LoadedAudioMutex{};
        std::mutex m_SoundBufferMutex{};
        std::mutex m_ActiveAudioMutex{};

        std::condition_variable m_ConditionVariable;
        bool m_bIsShuttingDown{ false };

        static constexpr int m_SoundEventBufferSize{ 15 };
        bool m_bAreAllSoundsMuted{ false };

        std::unordered_map<SoundID, std::unique_ptr<Audio>> m_LoadedAudio{};

        MIX_Mixer* m_Mixer{};
    };
}

AudioQueue::AudioQueue() :
    m_SoundEventBuffer{ m_SoundEventBufferSize }
{
    std::cout << "Initialized AudioQueue\n";

    if(!MIX_Init())
    {
        std::cout << FUNCTION_NAME << " Failed to initialize Mixer, Error: " << SDL_GetError() << '\n';
        return;
    }

    m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if(!m_Mixer)
    {
        std::cout << FUNCTION_NAME << " Failed to create mixer, Error: " << SDL_GetError() << '\n';
        return;
    }

    m_AudioThread = std::jthread([this](const std::stop_token& stopToken)
    {
        AudioThreadLoop(stopToken);
    });
}

AudioQueue::~AudioQueue()
{
    std::cout << FUNCTION_NAME << '\n';
    m_AudioThread.request_stop();
    m_ConditionVariable.notify_all();

    std::lock_guard mainLock(m_MainThreadMutex);
    std::lock_guard activeAudioLock(m_ActiveAudioMutex);
    std::lock_guard soundBufferLock(m_SoundBufferMutex);
    std::lock_guard loadedAudioLock(m_LoadedAudioMutex);

    for(const auto& uAudioClip : m_ActiveAudio | std::views::values)
    {
        uAudioClip->Stop();
    }

    CleanUpFinishedSounds();


    m_ActiveAudio.clear();
    m_LoadedAudio.clear();

    MIX_DestroyMixer(m_Mixer);
    m_Mixer = nullptr;
}

void AudioQueue::SendSoundEvent(const SoundEventData& soundEvent)
{
    std::lock_guard lock(m_SoundBufferMutex);

    // ignore request if audio is shutting down
    if(m_bIsShuttingDown)
    {
        return;
    }

    m_SoundEventBuffer.Push(soundEvent);
    m_ConditionVariable.notify_one();
}

const AudioClip* AudioQueue::GetAudioClip(const ActiveSoundID activeSoundId)
{
    std::lock_guard lock(m_ActiveAudioMutex);
    if(const auto it = m_ActiveAudio.find(activeSoundId); it != m_ActiveAudio.end())
    {
        return it->second.get();
    }

    return nullptr;
}

bool AudioQueue::IsAudioLoaded(const SoundID soundId)
{
    std::lock_guard lock(m_LoadedAudioMutex);
    return IsAudioLoadedUnLocked(soundId);
}

bool AudioQueue::IsAudioLoadedUnLocked(const SoundID soundId)
{
    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        return false;
    }

    if(const auto& audio = it->second; !audio)
    {
        return false;
    }

    return true;
}

void AudioQueue::AddAudio(SoundID soundId, const std::string& path)
{
    std::lock_guard lock(m_LoadedAudioMutex);
    m_LoadedAudio.insert({ soundId, std::make_unique<Audio>(path, m_Mixer) });
}


void AudioQueue::AudioThreadLoop(const std::stop_token& stopToken)
{
    while(!stopToken.stop_requested())
    {
        std::unique_lock lock(m_MainThreadMutex);
        m_ConditionVariable.wait(lock, [this, stopToken]
        {
            return stopToken.stop_requested() || !m_SoundEventBuffer.IsEmpty();
        });

        if(stopToken.stop_requested())
        {
            return;
        }

        while(!stopToken.stop_requested() && !m_SoundEventBuffer.IsEmpty())
        {
            std::unique_lock soundBufferLock(m_SoundBufferMutex);
            SoundEventData eventData{};
            m_SoundEventBuffer.Pop(eventData);
            soundBufferLock.unlock();

            std::lock_guard activeSoundLock(m_ActiveAudioMutex);
            ProcessSoundEvent(eventData);
        }

        // After all SoundEvents are done, clean any finished sounds
        std::lock_guard activeAudioLock(m_ActiveAudioMutex);
        CleanUpFinishedSounds();
    }
}

void AudioQueue::ProcessSoundEvent(const SoundEventData& eventData)
{
    SdlAudioClip* audioClip = nullptr;

    const auto it = std::ranges::find_if(m_ActiveAudio,
                                         [&eventData](auto& activeAudio)
                                         {
                                             const auto& [activeSoundID, uAudioClip] = activeAudio;
                                             if(activeSoundID.ID == eventData.ActiveSoundID.ID && uAudioClip)
                                             {
                                                 return true;
                                             }

                                             return false;
                                         });
    if(it != m_ActiveAudio.end())
    {
        audioClip = it->second.get();
    }

    const bool isAudioClipNeeded = IsAudioClipNeededForSoundEventType(eventData.Type);
    if(isAudioClipNeeded && !audioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! Audio clip not found but needed in SoundEvent, ActiveSoundID: " <<
                eventData.ActiveSoundID.ID << '\n';
        return;
    }

    if(eventData.SoundID.ID == -1 && IsValidSoundIDNeededForSoundEventType(eventData.Type))
    {
        std::cout << FUNCTION_NAME << " Failed! SoundId is invalid (-1) but needed in SoundEvent" << '\n';
        return;
    }

    switch(eventData.Type)
    {
        case SoundEventType::PlaySound:
        {
            // SoundClip already loaded, it shouldn't be loaded, bc Play shouldn't have an ActiveSoundID
            if(audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed to Play Sound, since it already has a sound playing" << '\n';
                return;
            }

            if(eventData.SoundID.ID == -1)
            {
                std::cout << FUNCTION_NAME << " Play: Invalid SoundID Given" << '\n';
                return;
            }

            const Audio* audio = GetAudio(eventData.SoundID);
            auto uAudioClip    = std::make_unique<
                SdlAudioClip>(eventData.ActiveSoundID, eventData.SoundID, m_Mixer, audio);

            if(!uAudioClip->Play())
            {
                std::cout << FUNCTION_NAME << " Play: Invalid SoundID Given" << '\n';
                return;
            }

            uAudioClip->SetVolume(eventData.Volume);

            if(m_bAreAllSoundsMuted)
            {
                uAudioClip->Mute();
            }

            m_ActiveAudio.insert(std::pair(eventData.ActiveSoundID, std::move(uAudioClip)));
        }
        break;
        case SoundEventType::StopSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->Stop();
        }
        break;
        case SoundEventType::ResumeSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->Resume();
        }
        break;
        case SoundEventType::PauseSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->Pause();
        }
        break;
        case SoundEventType::MuteSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->Mute();
        }
        break;
        case SoundEventType::UnMuteSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->UnMute();
        }
        break;
        case SoundEventType::LoopSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->Loop();
        }
        break;
        case SoundEventType::UnLoopSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->UnLoop();
        }
        break;
        case SoundEventType::SetVolumeSound:
        {
            if(!audioClip)
            {
                std::cout << FUNCTION_NAME << " Failed! AudioClip is invalid" << '\n';
                return;
            }
            audioClip->SetVolume(eventData.Volume);
        }
        break;

        case SoundEventType::StopSounds:
        {
            std::ranges::for_each(m_ActiveAudio | std::views::values,
                                  [&](const auto& currentAudioClip)
                                  {
                                      if(currentAudioClip->GetSoundId() == eventData.SoundID)
                                      {
                                          currentAudioClip->Stop();
                                      }
                                  });
        }
        break;
        case SoundEventType::ResumeSounds:
        {
            std::ranges::for_each(m_ActiveAudio | std::views::values,
                                  [&](const auto& currentAudioClip)
                                  {
                                      if(currentAudioClip->GetSoundId() == eventData.SoundID)
                                      {
                                          currentAudioClip->Resume();
                                      }
                                  });
        }
        break;
        case SoundEventType::PauseSounds:
        {
            std::ranges::for_each(m_ActiveAudio | std::views::values,
                                  [&](const auto& currentAudioClip)
                                  {
                                      if(currentAudioClip->GetSoundId() == eventData.SoundID)
                                      {
                                          currentAudioClip->Pause();
                                      }
                                  });
        }
        break;
        case SoundEventType::MuteSounds:
        {
            std::ranges::for_each(m_ActiveAudio | std::views::values,
                                  [&](const auto& currentAudioClip)
                                  {
                                      if(currentAudioClip->GetSoundId() == eventData.SoundID)
                                      {
                                          currentAudioClip->Mute();
                                      }
                                  });
        }
        break;
        case SoundEventType::UnMuteSounds:
        {
            std::ranges::for_each(m_ActiveAudio | std::views::values,
                                  [&](const auto& currentAudioClip)
                                  {
                                      if(currentAudioClip->GetSoundId() == eventData.SoundID)
                                      {
                                          currentAudioClip->UnMute();
                                      }
                                  });
        }
        break;


        case SoundEventType::StopAll:
        {
            std::ranges::for_each(m_ActiveAudio,
                                  [](auto& activeAudio)
                                  {
                                      if(!activeAudio.second)
                                      {
                                          return;
                                      }

                                      activeAudio.second->Stop();
                                  }
            );
        }
        break;
        case SoundEventType::ResumeAll:
        {
            std::ranges::for_each(m_ActiveAudio,
                                  [](auto& activeAudio)
                                  {
                                      if(!activeAudio.second)
                                      {
                                          return;
                                      }

                                      activeAudio.second->Resume();
                                  }
            );
        }
        break;
        case SoundEventType::PauseAll:
        {
            std::ranges::for_each(m_ActiveAudio,
                                  [](auto& activeAudio)
                                  {
                                      if(!activeAudio.second)
                                      {
                                          return;
                                      }

                                      activeAudio.second->Pause();
                                  }
            );
        }
        break;
        case SoundEventType::MuteAll:
        {
            m_bAreAllSoundsMuted = true;

            std::ranges::for_each(m_ActiveAudio,
                                  [](auto& activeAudio)
                                  {
                                      if(!activeAudio.second)
                                      {
                                          return;
                                      }

                                      activeAudio.second->Mute();
                                  }
            );
        }
        break;
        case SoundEventType::UnMuteAll:
        {
            m_bAreAllSoundsMuted = false;

            std::ranges::for_each(m_ActiveAudio,
                                  [](auto& activeAudio)
                                  {
                                      if(!activeAudio.second)
                                      {
                                          return;
                                      }

                                      activeAudio.second->UnMute();
                                  }
            );
        }
        break;
        case SoundEventType::SetVolumeAll:
        {
            std::ranges::for_each(m_ActiveAudio,
                                  [&eventData](auto& activeAudio)
                                  {
                                      if(!activeAudio.second)
                                      {
                                          return;
                                      }

                                      activeAudio.second->SetVolume(eventData.Volume);
                                  }
            );
        }
        break;
    }
}


void AudioQueue::CleanUpFinishedSounds()
{
    constexpr auto functionName = std::string_view(FUNCTION_NAME);

    std::erase_if(m_ActiveAudio,
                  [&functionName](const auto& activeAudio)
                  {
                      const auto& [activeSoundID, uAudioClip] = activeAudio;
                      if(uAudioClip->IsStopped())
                      {
                          std::cout << functionName << " Cleaning up ActiveSoundID: " << activeSoundID.ID << '\n';

                          return true;
                      }
                      return false;
                  });
}

Audio* AudioQueue::GetAudio(const SoundID soundId)
{
    std::lock_guard lock(m_LoadedAudioMutex);
    if(!IsAudioLoadedUnLocked(soundId))
    {
        return nullptr;
    }

    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        return nullptr;
    }

    return it->second.get();
}


constexpr bool AudioQueue::IsAudioClipNeededForSoundEventType(const SoundEventType& eventType)
{
    switch(eventType)
    {
        case SoundEventType::PlaySound:
            return false;

        case SoundEventType::StopSound:
        case SoundEventType::ResumeSound:
        case SoundEventType::PauseSound:
        case SoundEventType::MuteSound:
        case SoundEventType::UnMuteSound:
        case SoundEventType::LoopSound:
        case SoundEventType::UnLoopSound:
        case SoundEventType::SetVolumeSound:
            return true;

        case SoundEventType::StopSounds:
        case SoundEventType::ResumeSounds:
        case SoundEventType::PauseSounds:
        case SoundEventType::MuteSounds:
        case SoundEventType::UnMuteSounds:

        case SoundEventType::StopAll:
        case SoundEventType::ResumeAll:
        case SoundEventType::PauseAll:
        case SoundEventType::MuteAll:
        case SoundEventType::UnMuteAll:
        case SoundEventType::SetVolumeAll:
            return false;
    }

    const std::string errorMessage = std::string(FUNCTION_NAME) + " Failed! This line should never be reached" + '\n';

    std::cout << errorMessage;
    assert(false && errorMessage.c_str());
    return true;
}

constexpr bool AudioQueue::IsValidSoundIDNeededForSoundEventType(const SoundEventType& eventType)
{
    switch(eventType)
    {
        case SoundEventType::PlaySound:
            return true;

        case SoundEventType::StopSound:
        case SoundEventType::ResumeSound:
        case SoundEventType::PauseSound:
        case SoundEventType::MuteSound:
        case SoundEventType::UnMuteSound:
        case SoundEventType::LoopSound:
        case SoundEventType::UnLoopSound:
        case SoundEventType::SetVolumeSound:
            return false;

        case SoundEventType::StopSounds:
        case SoundEventType::ResumeSounds:
        case SoundEventType::PauseSounds:
        case SoundEventType::MuteSounds:
        case SoundEventType::UnMuteSounds:
            return true;

        case SoundEventType::StopAll:
        case SoundEventType::ResumeAll:
        case SoundEventType::PauseAll:
        case SoundEventType::MuteAll:
        case SoundEventType::UnMuteAll:
        case SoundEventType::SetVolumeAll:
            return false;
    }

    const std::string errorMessage = std::string(FUNCTION_NAME) + " Failed! This line should never be reached" + '\n';

    std::cout << errorMessage;
    assert(false && errorMessage.c_str());
    return true;
}


#pragma region SdlSoundSystem | NOT PIMPL


MixerSoundSystem::MixerSoundSystem()  = default;
MixerSoundSystem::~MixerSoundSystem() = default;


SoundID MixerSoundSystem::LoadSound(const std::string& path)
{
    return m_Pimpl->LoadSound(path);
}


ActiveSoundID MixerSoundSystem::Play(const SoundID soundId)
{
    return m_Pimpl->Play(soundId);
}

void MixerSoundSystem::Stop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Stop(activeSoundId);
}


void MixerSoundSystem::Resume(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Resume(activeSoundId);
}

void MixerSoundSystem::Pause(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Pause(activeSoundId);
}


void MixerSoundSystem::Mute(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Mute(activeSoundId);
}

void MixerSoundSystem::UnMute(const ActiveSoundID activeSoundId)
{
    m_Pimpl->UnMute(activeSoundId);
}

void MixerSoundSystem::Loop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Loop(activeSoundId);
}

void MixerSoundSystem::UnLoop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->UnLoop(activeSoundId);
}


void MixerSoundSystem::StopSounds(const SoundID soundId)
{
    m_Pimpl->StopSounds(soundId);
}

void MixerSoundSystem::ResumeSounds(const SoundID soundId)
{
    m_Pimpl->ResumeSounds(soundId);
}

void MixerSoundSystem::PauseSounds(const SoundID soundId)
{
    m_Pimpl->PauseSounds(soundId);
}

void MixerSoundSystem::MuteSounds(const SoundID soundId)
{
    m_Pimpl->MuteSounds(soundId);
}

void MixerSoundSystem::UnMuteSounds(const SoundID soundId)
{
    m_Pimpl->UnMuteSounds(soundId);
}


void MixerSoundSystem::ResumeAllSounds()
{
    m_Pimpl->ResumeAllSounds();
}

void MixerSoundSystem::PauseAllSounds()
{
    m_Pimpl->PauseAllSounds();
}

void MixerSoundSystem::StopAllSounds()
{
    m_Pimpl->StopAllSounds();
}

void MixerSoundSystem::MuteAllSounds()
{
    m_Pimpl->MuteAllSounds();
}

void MixerSoundSystem::UnMuteAllSounds()
{
    m_Pimpl->UnMuteAllSounds();
}

void MixerSoundSystem::SetVolumeAllSounds(const float volume)
{
    m_Pimpl->SetVolumeAllSounds(volume);
}


bool MixerSoundSystem::IsLoaded(const SoundID soundId)
{
    return m_Pimpl->IsLoaded(soundId);
}

bool MixerSoundSystem::IsPlaying(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsPlaying(activeSoundId);
}


bool MixerSoundSystem::IsPaused(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsPaused(activeSoundId);
}

bool MixerSoundSystem::IsMuted(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->IsMuted(activeSoundId);
}


float MixerSoundSystem::GetVolume(const ActiveSoundID activeSoundId)
{
    return m_Pimpl->GetVolume(activeSoundId);
}

void MixerSoundSystem::SetVolume(const ActiveSoundID activeSoundId, const float volume)
{
    m_Pimpl->SetVolume(activeSoundId, volume);
}


#pragma endregion


#pragma region SdlSoundSystem | PIMPL


MixerSoundSystem::Impl::Impl() :
    m_AudioQueue{ std::make_unique<AudioQueue>() }
{
}

MixerSoundSystem::Impl::~Impl()
{
    m_AudioQueue = nullptr;

    m_LoadedSoundIDs.clear();

    MIX_Quit();
}


SoundID MixerSoundSystem::Impl::LoadSound(const std::string& path)
{
    if(!std::filesystem::exists(path))
    {
        std::cout << FUNCTION_NAME << " Failed! Audio Path not found, Path: " << path << '\n';
        return SoundID{};
    }

    // if already loaded
    if(const auto it = m_LoadedSoundIDs.find(path); it != m_LoadedSoundIDs.end())
    {
        return it->second;
    }

    // Set before inserting so the soundId starts from 0, and not from 1
    SoundID soundId{ .ID = m_SoundIdValue };
    ++m_SoundIdValue;

    // load audio chunk
    m_LoadedSoundIDs.insert(std::pair(path, soundId));
    m_AudioQueue->AddAudio(soundId, path);

    return soundId;
}


ActiveSoundID MixerSoundSystem::Impl::Play(const SoundID soundId) const
{
    if(!m_AudioQueue->IsAudioLoaded(soundId))
    {
        return ActiveSoundID{ -1 };
    }

    // this also gives an error if not in the correct initialization order :D
    const SoundEventData data
    {
        .Type          = SoundEventType::PlaySound,
        .SoundID       = soundId,
        .ActiveSoundID = ActiveSoundID{ GetRandomNumber() },
        .Volume        = 1.f,
    };


    m_AudioQueue->SendSoundEvent(data);

    return data.ActiveSoundID;
}

void MixerSoundSystem::Impl::Stop(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::StopSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}


void MixerSoundSystem::Impl::Resume(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::ResumeSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::Pause(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::PauseSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}


void MixerSoundSystem::Impl::Mute(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::MuteSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::UnMute(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::UnMuteSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::Loop(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::LoopSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::UnLoop(const ActiveSoundID activeSoundId) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::UnLoopSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::StopSounds(const SoundID soundId) const
{
    const SoundEventData data
    {
        .Type    = SoundEventType::StopSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::ResumeSounds(const SoundID soundId) const
{
    const SoundEventData data
    {
        .Type    = SoundEventType::ResumeSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::PauseSounds(const SoundID soundId) const
{
    const SoundEventData data
    {
        .Type    = SoundEventType::PauseSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::MuteSounds(const SoundID soundId) const
{
    const SoundEventData data
    {
        .Type    = SoundEventType::MuteSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::UnMuteSounds(const SoundID soundId) const
{
    const SoundEventData data
    {
        .Type    = SoundEventType::UnMuteSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}


bool MixerSoundSystem::Impl::IsLoaded(const SoundID soundId) const
{
    return m_AudioQueue->IsAudioLoaded(soundId);
}

bool MixerSoundSystem::Impl::IsPlaying(const ActiveSoundID activeSoundId) const
{
    // this is special bc we are sending request, and you can't immediately get a response back
    // OR
    // we don't use the audio queue's thread and get the m_ActiveSound's or something like that, ...

    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    return pAudioClip->IsPlaying();
}


bool MixerSoundSystem::Impl::IsPaused(const ActiveSoundID activeSoundId) const
{
    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    return pAudioClip->IsPaused();
}

bool MixerSoundSystem::Impl::IsMuted(const ActiveSoundID activeSoundId) const
{
    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    return pAudioClip->IsMuted();
}


float MixerSoundSystem::Impl::GetVolume(const ActiveSoundID activeSoundId) const
{
    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return 0.f;
    }

    return pAudioClip->GetVolume();
}

void MixerSoundSystem::Impl::SetVolume(const ActiveSoundID activeSoundId, const float volume) const
{
    const SoundEventData data
    {
        .Type          = SoundEventType::SetVolumeSound,
        .ActiveSoundID = activeSoundId,
        .Volume        = volume
    };

    m_AudioQueue->SendSoundEvent(data);
}


void MixerSoundSystem::Impl::ResumeAllSounds() const
{
    const SoundEventData data
    {
        .Type = SoundEventType::ResumeAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::PauseAllSounds() const
{
    const SoundEventData data
    {
        .Type = SoundEventType::PauseAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::StopAllSounds() const
{
    const SoundEventData data
    {
        .Type = SoundEventType::StopAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::MuteAllSounds() const
{
    const SoundEventData data
    {
        .Type = SoundEventType::MuteAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::UnMuteAllSounds() const
{
    const SoundEventData data
    {
        .Type = SoundEventType::UnMuteAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void MixerSoundSystem::Impl::SetVolumeAllSounds(const float volume) const
{
    const SoundEventData data
    {
        .Type   = SoundEventType::SetVolumeAll,
        .Volume = volume
    };

    m_AudioQueue->SendSoundEvent(data);
}


#pragma endregion

