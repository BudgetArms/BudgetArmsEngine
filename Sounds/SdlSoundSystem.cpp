#include "SdlSoundSystem.h"

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
#include "Sounds/AudioClip.h"
#include "Wrappers/Audio.h"


using namespace bae;

namespace bae
{
    class AudioQueue;
}

class SdlSoundSystem::Impl
{
public:
    Impl();
    ~Impl();


    [[nodiscard]] SoundID LoadSound(const std::string& path);

    ActiveSoundID Play(SoundID soundId);


    void Stop(ActiveSoundID activeSoundId) const;
    void Resume(ActiveSoundID activeSoundId) const;
    void Pause(ActiveSoundID activeSoundId) const;
    void Mute(ActiveSoundID activeSoundId) const;
    void UnMute(ActiveSoundID activeSoundId) const;
    void Loop(ActiveSoundID activeSoundId) const;
    void UnLoop(ActiveSoundID activeSoundId) const;


    // SoundID
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

    [[nodiscard]] Audio* GetAudio(SoundID soundId);
    [[nodiscard]] MIX_Mixer* GetMixer() const;

private:
    std::unordered_map<std::string, SoundID> m_LoadedSoundIDs{};
    std::unordered_map<SoundID, std::unique_ptr<Audio>> m_LoadedAudio{};

    std::unique_ptr<AudioQueue> m_AudioQueue{};

    MIX_Mixer* m_Mixer{};
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

    private:
        void AudioThreadLoop(const std::stop_token& stopToken);

        void ProcessSoundEvent(const SoundEventData& eventData);
        void CleanUpFinishedSounds();

        [[nodiscard]] static constexpr bool IsAudioClipNeededForSoundEventType(const SoundEventType& eventType);
        [[nodiscard]] static constexpr bool IsValidSoundIDNeededForSoundEventType(const SoundEventType& eventType);


        RingBuffer<SoundEventData> m_SoundEventBuffer;
        std::unordered_map<ActiveSoundID, std::unique_ptr<SdlAudioClip>> m_ActiveAudio;

        std::jthread m_AudioThread;

        std::mutex m_Mutex;
        std::condition_variable m_ConditionVariable;
        bool m_bIsShuttingDown = false;

        static constexpr int m_SoundEventBufferSize{ 15 };
        bool m_bAreAllSoundsMuted{ false };
    };
}

AudioQueue::AudioQueue() :
    m_SoundEventBuffer{ m_SoundEventBufferSize }
{
    std::cout << "Initialized AudioQueue\n";

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

    std::lock_guard lock(m_Mutex);

    for(const auto& uAudioClip : m_ActiveAudio | std::views::values)
    {
        uAudioClip->Stop();
    }

    CleanUpFinishedSounds();

    m_ActiveAudio.clear();
}

void AudioQueue::SendSoundEvent(const SoundEventData& soundEvent)
{
    std::lock_guard lock(m_Mutex);

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
    std::lock_guard lock(m_Mutex);
    if(const auto it = m_ActiveAudio.find(activeSoundId); it != m_ActiveAudio.end())
    {
        return it->second.get();
    }

    return nullptr;
}


void AudioQueue::AudioThreadLoop(const std::stop_token& stopToken)
{
    while(!stopToken.stop_requested())
    {
        std::unique_lock lock(m_Mutex);
        m_ConditionVariable.wait(lock, [this, stopToken]
        {
            return stopToken.stop_requested() || !m_SoundEventBuffer.IsEmpty();
        });

        if(stopToken.stop_requested())
        {
            return;
        }

        while(!m_SoundEventBuffer.IsEmpty())
        {
            SoundEventData eventData{};
            m_SoundEventBuffer.Pop(eventData);

            if(stopToken.stop_requested())
            {
                return;
            }

            lock.unlock();

            ProcessSoundEvent(eventData);

            lock.lock();
        }

        // After all SoundEvents are done, clean any finished sounds
        CleanUpFinishedSounds();
    }
}

void AudioQueue::ProcessSoundEvent(const SoundEventData& eventData)
{
    SdlAudioClip* audioClip = nullptr;

    // TODO: use algorithms??
    if(const auto it = m_ActiveAudio.find(eventData.ActiveSoundID); it != m_ActiveAudio.end())
    {
        if(it->second)
        {
            audioClip = it->second.get();
        }
    }

    const bool isAudioClipNeeded = IsAudioClipNeededForSoundEventType(eventData.Type);
    if(isAudioClipNeeded && !audioClip && eventData.Type != SoundEventType::PlaySound)
    {
        std::cout << FUNCTION_NAME << " Failed! Audio clip not found but needed in SoundEvent" << '\n';
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

            auto uAudioClip = std::make_unique<SdlAudioClip>(eventData.ActiveSoundID, eventData.SoundID);

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
            // TODO: I could used std::ranges::for_each here
            for(const std::unique_ptr<SdlAudioClip>& currentAudioClip : m_ActiveAudio | std::views::values)
            {
                if(currentAudioClip->GetSoundId() == eventData.SoundID)
                {
                    currentAudioClip->Stop();
                }
            }
        }
        break;
        case SoundEventType::ResumeSounds:
        {
            for(const std::unique_ptr<SdlAudioClip>& currentAudioClip : m_ActiveAudio | std::views::values)
            {
                if(currentAudioClip->GetSoundId() == eventData.SoundID)
                {
                    currentAudioClip->Resume();
                }
            }
        }
        break;
        case SoundEventType::PauseSounds:
        {
            for(const std::unique_ptr<SdlAudioClip>& currentAudioClip : m_ActiveAudio | std::views::values)
            {
                if(currentAudioClip->GetSoundId() == eventData.SoundID)
                {
                    currentAudioClip->Pause();
                }
            }
        }
        break;
        case SoundEventType::MuteSounds:
        {
            for(const std::unique_ptr<SdlAudioClip>& currentAudioClip : m_ActiveAudio | std::views::values)
            {
                if(currentAudioClip->GetSoundId() == eventData.SoundID)
                {
                    currentAudioClip->Mute();
                }
            }
        }
        break;
        case SoundEventType::UnMuteSounds:
        {
            for(const std::unique_ptr<SdlAudioClip>& currentAudioClip : m_ActiveAudio | std::views::values)
            {
                if(currentAudioClip->GetSoundId() == eventData.SoundID)
                {
                    currentAudioClip->UnMute();
                }
            }
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
    for(const auto& [activeSoundID, uAudioClip] : m_ActiveAudio)
    {
        if(uAudioClip->IsStopped())
        {
            std::cout << FUNCTION_NAME << " Cleaning up SoundID: " << activeSoundID.ID << '\n';
        }
    }

    std::erase_if(m_ActiveAudio,
                  [](auto& activeAudio)
                  {
                      return activeAudio.second->IsStopped();
                  }
    );
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


SdlSoundSystem::SdlSoundSystem()  = default;
SdlSoundSystem::~SdlSoundSystem() = default;


SoundID SdlSoundSystem::LoadSound(const std::string& path)
{
    return m_Pimpl->LoadSound(path);
}


ActiveSoundID SdlSoundSystem::Play(const SoundID soundId)
{
    return m_Pimpl->Play(soundId);
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

void SdlSoundSystem::Loop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->Loop(activeSoundId);
}

void SdlSoundSystem::UnLoop(const ActiveSoundID activeSoundId)
{
    m_Pimpl->UnLoop(activeSoundId);
}


void SdlSoundSystem::StopSounds(const SoundID soundId)
{
    m_Pimpl->StopSounds(soundId);
}

void SdlSoundSystem::ResumeSounds(const SoundID soundId)
{
    m_Pimpl->ResumeSounds(soundId);
}

void SdlSoundSystem::PauseSounds(const SoundID soundId)
{
    m_Pimpl->PauseSounds(soundId);
}

void SdlSoundSystem::MuteSounds(const SoundID soundId)
{
    m_Pimpl->MuteSounds(soundId);
}

void SdlSoundSystem::UnMuteSounds(const SoundID soundId)
{
    m_Pimpl->UnMuteSounds(soundId);
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


SdlSoundSystem::Impl::Impl() :
    m_AudioQueue{ std::make_unique<AudioQueue>() }
{
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
}

SdlSoundSystem::Impl::~Impl()
{
    m_AudioQueue = nullptr;

    MIX_DestroyMixer(m_Mixer);
    m_Mixer = nullptr;

    m_LoadedSoundIDs.clear();
    m_LoadedAudio.clear();

    MIX_Quit();
}


SoundID SdlSoundSystem::Impl::LoadSound(const std::string& path)
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
    SoundID soundId{ .ID = static_cast<int>(m_LoadedAudio.size()) };

    // load audio chunk
    m_LoadedSoundIDs.insert(std::pair(path, soundId));
    m_LoadedAudio.insert(std::pair(soundId, std::make_unique<Audio>(path)));

    return soundId;
}


ActiveSoundID SdlSoundSystem::Impl::Play(const SoundID soundId)
{
    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        std::cout << FUNCTION_NAME << " Failed! Sound is not loaded, soundId: " << soundId.ID << '\n';
        return ActiveSoundID{ -1 };
    }

    if(const auto& audio = it->second; !audio)
    {
        std::cout << FUNCTION_NAME << " Failed! Audio is nullptr, SoundId: " << soundId.ID << '\n';
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

void SdlSoundSystem::Impl::Stop(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::StopSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}


void SdlSoundSystem::Impl::Resume(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::ResumeSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::Pause(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::PauseSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}


void SdlSoundSystem::Impl::Mute(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type          = SoundEventType::MuteSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMute(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type          = SoundEventType::UnMuteSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::Loop(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type          = SoundEventType::LoopSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnLoop(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type          = SoundEventType::UnLoopSound,
        .ActiveSoundID = activeSoundId,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::StopSounds(const SoundID soundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, SoundID: " << soundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type    = SoundEventType::StopSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::ResumeSounds(const SoundID soundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, SoundID: " << soundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type    = SoundEventType::ResumeSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::PauseSounds(const SoundID soundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, SoundID: " << soundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type    = SoundEventType::PauseSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::MuteSounds(const SoundID soundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, SoundID: " << soundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type    = SoundEventType::MuteSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMuteSounds(const SoundID soundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, SoundID: " << soundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type    = SoundEventType::UnMuteSounds,
        .SoundID = soundId
    };

    m_AudioQueue->SendSoundEvent(data);
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
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    return pAudioClip->IsPlaying();
}


bool SdlSoundSystem::Impl::IsPaused(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }


    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    return pAudioClip->IsPaused();
}

bool SdlSoundSystem::Impl::IsMuted(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }


    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return false;
    }

    return pAudioClip->IsMuted();
}


float SdlSoundSystem::Impl::GetVolume(const ActiveSoundID activeSoundId) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return 0.f;
    }

    const auto pAudioClip = m_AudioQueue->GetAudioClip(activeSoundId);
    if(!pAudioClip)
    {
        std::cout << FUNCTION_NAME << " Failed! AudioClip not found, ActiveSoundID: " << activeSoundId.ID << '\n';
        return 0.f;
    }

    return pAudioClip->GetVolume();
}

void SdlSoundSystem::Impl::SetVolume(const ActiveSoundID activeSoundId, const float volume) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, ActiveSoundID: " << activeSoundId.ID << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type          = SoundEventType::SetVolumeSound,
        .ActiveSoundID = activeSoundId,
        .Volume        = volume
    };

    m_AudioQueue->SendSoundEvent(data);
}


void SdlSoundSystem::Impl::ResumeAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded" << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type = SoundEventType::ResumeAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::PauseAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type = SoundEventType::PauseAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::StopAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type = SoundEventType::StopAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::MuteAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded" << '\n';
        return;
    }


    const SoundEventData data
    {
        .Type = SoundEventType::MuteAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMuteAllSounds() const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded" << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type = SoundEventType::UnMuteAll,
    };

    m_AudioQueue->SendSoundEvent(data);
}

void SdlSoundSystem::Impl::SetVolumeAllSounds(const float volume) const
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded" << '\n';
        return;
    }

    const SoundEventData data
    {
        .Type   = SoundEventType::SetVolumeAll,
        .Volume = volume
    };

    m_AudioQueue->SendSoundEvent(data);
}

Audio* SdlSoundSystem::Impl::GetAudio(const SoundID soundId)
{
    if(m_LoadedAudio.empty())
    {
        std::cout << FUNCTION_NAME << " Failed! No Sounds are loaded, SoundID: " << soundId.ID << '\n';
        return nullptr;
    }

    const auto it = m_LoadedAudio.find(soundId);
    if(it == m_LoadedAudio.end())
    {
        std::cout << FUNCTION_NAME << " Failed! Audio not found, SoundID: " << soundId.ID << '\n';
        return nullptr;
    }

    return it->second.get();
}

MIX_Mixer* SdlSoundSystem::Impl::GetMixer() const
{
    return m_Mixer;
}


#pragma endregion

