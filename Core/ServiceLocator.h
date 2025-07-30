#pragma once
#include <memory>
#include "Core/SoundSystem.h"
#include "Core/NullSoundSystem.h"
#include "Core/AudioQueue.h"


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


		template<typename T>
		static AudioQueue<T>& GetAudioQueue() { return *m_sAudioQueueInstance; }

		template<typename AudioClipType,
			typename = std::enable_if_t<std::is_base_of_v<bae::AudioClip, AudioClipType> &&
			!std::is_same_v<bae::AudioClip, AudioClipType>>>
			static void RegisterAudioQueue()
		{
			m_sAudioQueueInstance = std::make_unique<AudioQueue<AudioClipType>>();
		}


	private:
		static std::unique_ptr<SoundSystem> m_sSoundSystemInstance;
		template<typename T>
		static std::unique_ptr<AudioQueue<T>> m_sAudioQueueInstance;


	};
}


