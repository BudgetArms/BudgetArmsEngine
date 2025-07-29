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

		static AudioQueue& GetAudioQueue() { return *m_sAudioQueueInstance; }


	private:
		static std::unique_ptr<SoundSystem> m_sSoundSystemInstance;
		static std::unique_ptr<AudioQueue> m_sAudioQueueInstance;


	};
}


