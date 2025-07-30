#pragma once

#include <memory>

#include "Sounds/SoundSystem.h"
#include "Sounds/NullSoundSystem.h"
#include "Sounds/AudioQueue.h"


namespace bae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_sSoundSystemInstance; }

		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
		{
			if (!soundSystem)
				m_sSoundSystemInstance = std::make_unique<NullSoundSystem>();
			else
				m_sSoundSystemInstance = std::move(soundSystem);
		}


		static IAudioQueue& GetAudioQueue() { return *m_sAudioQueueInstance; }

		template<typename AudioClipType,
			typename = std::enable_if_t<std::is_base_of_v<bae::AudioClip, AudioClipType> &&
			!std::is_same_v<bae::AudioClip, AudioClipType>>>
			static void RegisterAudioQueue()
		{
			m_sAudioQueueInstance = std::make_unique<AudioQueue<AudioClipType>>();
		}


	private:
		static std::unique_ptr<SoundSystem> m_sSoundSystemInstance;
		static std::unique_ptr<IAudioQueue> m_sAudioQueueInstance;


	};
}


