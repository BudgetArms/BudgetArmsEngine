#include "AudioQueue.h"

#include <iostream>
#include <ranges>
#include <algorithm>

#include "Core/AudioClip.h"
#include "Core/SdlAudioClip.h"
#include "Core/HelperFunctions.h"


using namespace bae;


/*

AudioQueue::AudioQueue() :
	m_SoundEventBuffer{ 10 }
{
	std::cout << "Initialized AudioQueue\n";
	m_AudioThread = std::thread(&AudioQueue::AudioThreadLoop, this);
}

AudioQueue::~AudioQueue()
{
	m_bQuit = true;
	if (m_AudioThread.joinable())
		m_AudioThread.join();

	for (auto& [id, sound] : m_ActiveAudio)
		sound->Stop();

	m_ActiveAudio.clear();
}


void AudioQueue::SendSoundEvent(const SoundEventData& soundEvent)
{
	m_SoundEventBuffer.Push(soundEvent);
}

const AudioClip* AudioQueue::GetAudioClip(ActiveSoundID activeSoundId)
{
	if (auto it = m_ActiveAudio.find(activeSoundId); it != m_ActiveAudio.end())
		return it->second.get();

	return nullptr;
}


void AudioQueue::AudioThreadLoop()
{
	SoundEventData eventData;

	while (!m_bQuit)
	{
		while (m_SoundEventBuffer.Pop(eventData))
			ProcessSoundEvent(eventData);

		CleanUpFinishedSounds();

		std::this_thread::sleep_for(std::chrono::microseconds(static_cast<long long>(m_ThreadSleepTimeMilliSec)));

	}
}


void AudioQueue::ProcessSoundEvent(const SoundEventData& eventData)
{
	AudioClip* audioClip = nullptr;

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

			auto uAudioClip = std::make_unique<bae::SdlAudioClip>(eventData.ActiveSoundID, eventData.SoundID);

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


void AudioQueue::CleanUpFinishedSounds()
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


//*/

