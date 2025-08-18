#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <ranges>
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
	class AudioQueue : public IAudioQueue
	{
		static_assert(std::is_base_of<bae::AudioClip, AudioClipType>::value, "AudioClipType must derive from AudioClip");


	public:
		AudioQueue();
		virtual ~AudioQueue();

		virtual void SendSoundEvent(const SoundEventData& soundEvent) override;
		const AudioClip* GetAudioClip(ActiveSoundID activeSoundId) override;


	private:
		void AudioThreadLoop();

		void ProcessSoundEvent(const SoundEventData& eventData);
		void CleanUpFinishedSounds();


		RingBuffer<SoundEventData> m_SoundEventBuffer;
		std::unordered_map<ActiveSoundID, std::unique_ptr<AudioClip>> m_ActiveAudio;

		std::thread m_AudioThread;

		static const int m_SoundEventBufferSize{ 15 };
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
bae::AudioQueue<AudioClipType>::~AudioQueue()
{
	m_bQuit = true;
	if (m_AudioThread.joinable())
		m_AudioThread.join();

	for (auto& [id, sound] : m_ActiveAudio)
		sound->Stop();

	m_ActiveAudio.clear();
}


template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::SendSoundEvent(const SoundEventData& soundEvent)
{
	m_SoundEventBuffer.Push(soundEvent);
}

template<typename AudioClipType>
const bae::AudioClip* bae::AudioQueue<AudioClipType>::GetAudioClip(ActiveSoundID activeSoundId)
{
	if (auto it = m_ActiveAudio.find(activeSoundId); it != m_ActiveAudio.end())
		return it->second.get();

	return nullptr;
}


template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::AudioThreadLoop()
{

	while (!m_bQuit)
	{
		SoundEventData eventData;
		while (m_SoundEventBuffer.Pop(eventData))
			ProcessSoundEvent(eventData);

		CleanUpFinishedSounds();

		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(m_ThreadSleepTimeMilliSec)));

	}
}


template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::ProcessSoundEvent(const SoundEventData& eventData)
{
	bae::AudioClip* audioClip = nullptr;

	if (auto it = m_ActiveAudio.find(eventData.ActiveSoundID); it != m_ActiveAudio.end())
		if (it->second)
			audioClip = it->second.get();


	switch (eventData.Type)
	{
		case bae::SoundEventType::Play:
		{
			// SoundClip already loaded, it shouldn't be loaded, bc Play shouldn't have an ActiveSoundID
			if (audioClip)
				return;

			if (eventData.SoundID.ID == -1)
			{
				std::cout << GetFunctionName() << " Play: Invalid SoundID Given" << '\n';
				return;
			}

			std::unique_ptr<AudioClip> uAudioClip = std::make_unique<AudioClipType>(eventData.ActiveSoundID, eventData.SoundID);

			// if the channels are full it returns -1
			if (!uAudioClip->Play())
				return;

			uAudioClip->SetVolume(eventData.Volume);

			if (m_bAreAllSoundsMuted)
				uAudioClip->Mute();

			m_ActiveAudio.insert(std::pair(eventData.ActiveSoundID, std::move(uAudioClip)));

		} break;
		case bae::SoundEventType::Stop:
		{
			if (!audioClip)
				return;

			audioClip->Stop();
		} break;
		case bae::SoundEventType::Resume:
		{
			if (!audioClip)
				return;

			audioClip->Resume();
		} break;
		case bae::SoundEventType::Pause:
		{
			if (!audioClip)
				return;

			audioClip->Pause();
		} break;
		case bae::SoundEventType::Mute:
		{
			if (!audioClip)
				return;

			audioClip->Mute();
		} break;
		case bae::SoundEventType::UnMute:
		{
			if (!audioClip)
				return;

			audioClip->UnMute();
		} break;
		case bae::SoundEventType::SetVolume:
		{
			if (!audioClip)
				return;

			audioClip->SetVolume(eventData.Volume);
		} break;
		case bae::SoundEventType::StopAll:
		{
			std::ranges::for_each(m_ActiveAudio,
				[](auto& activeAudio)
				{
					if (!activeAudio.second)
						return;

					activeAudio.second->Stop();
				}
			);

		} break;
		case bae::SoundEventType::ResumeAll:
		{
			std::ranges::for_each(m_ActiveAudio,
				[](auto& activeAudio)
				{
					if (!activeAudio.second)
						return;

					activeAudio.second->Resume();
				}
			);

		} break;
		case bae::SoundEventType::PauseAll:
		{
			std::ranges::for_each(m_ActiveAudio,
				[](auto& activeAudio)
				{
					if (!activeAudio.second)
						return;

					activeAudio.second->Pause();
				}
			);

		} break;
		case bae::SoundEventType::MuteAll:
		{
			m_bAreAllSoundsMuted = true;

			std::ranges::for_each(m_ActiveAudio,
				[](auto& activeAudio)
				{
					if (!activeAudio.second)
						return;

					activeAudio.second->Mute();
				}
			);

		} break;
		break;
		case bae::SoundEventType::UnMuteAll:
		{
			m_bAreAllSoundsMuted = false;

			std::ranges::for_each(m_ActiveAudio,
				[](auto& activeAudio)
				{
					if (!activeAudio.second)
						return;

					activeAudio.second->UnMute();
				}
			);

		} break;
		case bae::SoundEventType::SetVolumeAll:
		{
			std::ranges::for_each(m_ActiveAudio,
				[&eventData](auto& activeAudio)
				{
					if (!activeAudio.second)
						return;

					activeAudio.second->SetVolume(eventData.Volume);
				}
			);

		} break;

	}
}


template<typename AudioClipType>
void bae::AudioQueue<AudioClipType>::CleanUpFinishedSounds()
{

	for (auto& activeAudio : m_ActiveAudio)
		if (!activeAudio.second->IsPlaying())
			std::cout << "AudioQueueLogger::CleanUp SoundID: " << activeAudio.first.ID << '\n';

	std::erase_if(m_ActiveAudio,
		[](auto& activeAudio)
		{
			return !activeAudio.second->IsPlaying();
		}
	);

}


