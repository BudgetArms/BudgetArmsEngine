#pragma once

#include <memory>
#include <unordered_map>

#include "Core/SoundSystem.h"
#include "Core/RingBuffer.h"
//#include "Core/SdlAudioClip.h"


namespace bae
{
	class AudioClip;

	enum class SoundEventType
	{
		Play,
		Stop,
		Resume,
		Pause,
		Mute,
		UnMute,
		SetVolume,

		StopAll,
		ResumeAll,
		PauseAll,
		MuteAll,
		UnMuteAll,
		SetVolumeAll,
	};

	struct SoundEventData
	{
		SoundEventType Type;
		SoundID SoundID{ -1 };
		ActiveSoundID ActiveSoundID{ -1 };
		float Volume{ 1 };
	};


	class AudioQueue
	{
	public:
		AudioQueue();
		~AudioQueue();

		void SendSoundEvent(const SoundEventData& soundEvent);
		const AudioClip* GetAudioClip(ActiveSoundID activeSoundId);

		bool m_bQuit{ false };


	private:
		void AudioThreadLoop();

		void ProcessSoundEvent(const SoundEventData& eventData);
		void CleanUpFinishedSounds();


		RingBuffer<SoundEventData> m_SoundEventBuffer;
		std::unordered_map<ActiveSoundID, std::unique_ptr<AudioClip>> m_ActiveAudio;

		std::thread m_AudioThread;

		const float m_ThreadSleepTimeMilliSec{ 100.f };
		bool m_bAreAllSoundsMuted{ false };


	};

}


