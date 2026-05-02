#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <thread>
#include <unordered_map>

#include "Core/HelperFunctions.h"
#include "Core/RingBuffer.h"
#include "Sounds/AudioClip.h"
#include "Sounds/SoundStructs.h"


namespace bae
{
    class IAudioQueue
    {
    public:
        virtual ~IAudioQueue() = default;
        virtual void SendSoundEvent(const SoundEventData& soundEvent) = 0;
        virtual const AudioClip* GetAudioClip(ActiveSoundID activeSoundId) = 0;
    };


    template<typename AudioClipType>
    class AudioQueue final : public IAudioQueue
    {
        static_assert(std::is_base_of_v<AudioClip, AudioClipType>, "AudioClipType must derive from AudioClip");

    public:
        AudioQueue();

        ~AudioQueue() override
        {
            m_bQuit = true;
            if(m_AudioThread.joinable())
            {
                m_AudioThread.join();
            }

            for(const auto& uAudioClip : m_ActiveAudio | std::views::values)
            {
                uAudioClip->Stop();
            }

            m_ActiveAudio.clear();
        }

        void SendSoundEvent(const SoundEventData& soundEvent) override;
        const AudioClip* GetAudioClip(ActiveSoundID activeSoundId) override;

    private:
        void AudioThreadLoop();

        void ProcessSoundEvent(const SoundEventData& eventData);
        void CleanUpFinishedSounds();


        RingBuffer<SoundEventData> m_SoundEventBuffer;
        std::unordered_map<ActiveSoundID, std::unique_ptr<AudioClip>> m_ActiveAudio;

        std::thread m_AudioThread;

        static constexpr int m_SoundEventBufferSize{ 15 };
        const float m_ThreadSleepTimeMilliSec{ 100.f };
        bool m_bAreAllSoundsMuted{ false };
        bool m_bQuit{ false };
    };
}


template<typename AudioClipType>
bae::AudioQueue<AudioClipType>::AudioQueue() :
    m_SoundEventBuffer{ m_SoundEventBufferSize }
{
    std::cout << "Initialized AudioQueue\n";
    m_AudioThread = std::thread(&AudioQueue::AudioThreadLoop, this);
}

template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::SendSoundEvent(const SoundEventData& soundEvent)
{
    m_SoundEventBuffer.Push(soundEvent);
}

template<typename AudioClipType>
const bae::AudioClip* bae::AudioQueue<AudioClipType>::GetAudioClip(const ActiveSoundID activeSoundId)
{
    if(const auto it = m_ActiveAudio.find(activeSoundId); it != m_ActiveAudio.end())
    {
        return it->second.get();
    }

    return nullptr;
}


// TODO: use cd (condition variable)
template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::AudioThreadLoop()
{
    while(!m_bQuit)
    {
        SoundEventData eventData;
        while(m_SoundEventBuffer.Pop(eventData))
        {
            ProcessSoundEvent(eventData);
        }

        CleanUpFinishedSounds();

        std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(m_ThreadSleepTimeMilliSec)));
    }
}


template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::ProcessSoundEvent(const SoundEventData& eventData)
{
    AudioClip* audioClip = nullptr;

    // TODO: use algorithms??
    if(const auto it = m_ActiveAudio.find(eventData.ActiveSoundID); it != m_ActiveAudio.end())
    {
        if(it->second)
        {
            audioClip = it->second.get();
        }
    }

    const bool isAudioClipNeeded = IsAudioClipNeededForSoundEventType(eventData.Type);
    if(isAudioClipNeeded && !audioClip && eventData.Type != SoundEventType::Play)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed, Audio clip not found but needed in SoundEvent"));
        return;
    }

    switch(eventData.Type)
    {
        case SoundEventType::Play:
        {
            // SoundClip already loaded, it shouldn't be loaded, bc Play shouldn't have an ActiveSoundID
            if(audioClip)
            {
                // TODO: improve error message
                throw std::runtime_error(
                    FUNCTION_NAME + std::string(" Failed to Play Sound, since it already has a sound playing"));
                return;
            }

            if(eventData.SoundID.ID == -1)
            {
                std::cout << FUNCTION_NAME << " Play: Invalid SoundID Given" << '\n';
                return;
            }

            std::unique_ptr<AudioClip> uAudioClip = std::make_unique<AudioClipType>(
                eventData.ActiveSoundID, eventData.SoundID);

            // if the channels are full it returns -1
            if(!uAudioClip->Play())
            {
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
        case SoundEventType::Stop:
        {
            audioClip->Stop();
        }
        break;
        case SoundEventType::Resume:
        {
            audioClip->Resume();
        }
        break;
        case SoundEventType::Pause:
        {
            audioClip->Pause();
        }
        break;
        case SoundEventType::Mute:
        {
            audioClip->Mute();
        }
        break;
        case SoundEventType::UnMute:
        {
            audioClip->UnMute();
        }
        break;
        case SoundEventType::SetVolume:
        {
            audioClip->SetVolume(eventData.Volume);
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


template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::CleanUpFinishedSounds()
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


