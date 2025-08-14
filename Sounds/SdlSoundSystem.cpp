#include "SdlSoundSystem.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <memory>

#include <SDL.h>
#include <SDL_mixer.h>

#include "Core/HelperFunctions.h"
#include "Core/ServiceLocator.h"
#include "Sounds/AudioClip.h"
#include "Sounds/NullAudioClip.h"
#include "Wrappers/AudioChunk.h"



using namespace bae;


class SdlSoundSystem::Impl
{
public:
	Impl();
	~Impl();


	SoundID LoadSound(const std::string& path);

	ActiveSoundID Play(SoundID soundId, float volume);
	void Stop(ActiveSoundID activeSoundId);

	void Resume(ActiveSoundID activeSoundId);
	void Pause(ActiveSoundID activeSoundId);

	void Mute(ActiveSoundID activeSoundId);
	void UnMute(ActiveSoundID activeSoundId);

	bool IsLoaded(SoundID soundId);
	bool IsPlaying(ActiveSoundID activeSoundId);

	bool IsPaused(ActiveSoundID activeSoundId);
	bool IsMuted(ActiveSoundID activeSoundId);

	float GetVolume(ActiveSoundID activeSoundId);
	void SetVolume(ActiveSoundID activeSoundId, float volume);


	void ResumeAllSounds();
	void PauseAllSounds();
	void StopAllSounds();
	void MuteAllSounds();
	void UnMuteAllSounds();

	void SetVolumeAllSounds(float volume);


	AudioChunk* GetChunk(SoundID soundId);


private:
	static const int m_NrChannels{ 12 };

	std::unordered_map<std::string, SoundID> m_LoadedSoudIDs;
	std::unordered_map<SoundID, std::unique_ptr<AudioChunk>> m_LoadedAudio;


};



#pragma region SdlSoundSystem | NOT PIMPL


SdlSoundSystem::SdlSoundSystem()
{
}

SdlSoundSystem::~SdlSoundSystem()
{
}


SoundID SdlSoundSystem::LoadSound(const std::string& path)
{
	return m_Pimpl->LoadSound(path);
}


ActiveSoundID SdlSoundSystem::Play(SoundID soundId, float volume)
{
	return m_Pimpl->Play(soundId, volume);
}

void SdlSoundSystem::Stop(ActiveSoundID activeSoundId)
{
	m_Pimpl->Stop(activeSoundId);
}


void SdlSoundSystem::Resume(ActiveSoundID activeSoundId)
{
	m_Pimpl->Resume(activeSoundId);
}

void SdlSoundSystem::Pause(ActiveSoundID activeSoundId)
{
	m_Pimpl->Pause(activeSoundId);
}


void SdlSoundSystem::Mute(ActiveSoundID activeSoundId)
{
	m_Pimpl->Mute(activeSoundId);
}

void SdlSoundSystem::UnMute(ActiveSoundID activeSoundId)
{
	m_Pimpl->UnMute(activeSoundId);
}


bool SdlSoundSystem::IsLoaded(SoundID soundId)
{
	return m_Pimpl->IsLoaded(soundId);
}

bool SdlSoundSystem::IsPlaying(ActiveSoundID activeSoundId)
{
	return m_Pimpl->IsPlaying(activeSoundId);
}


bool SdlSoundSystem::IsPaused(ActiveSoundID activeSoundId)
{
	return m_Pimpl->IsPaused(activeSoundId);
}

bool SdlSoundSystem::IsMuted(ActiveSoundID activeSoundId)
{
	return m_Pimpl->IsMuted(activeSoundId);
}


float SdlSoundSystem::GetVolume(ActiveSoundID activeSoundId)
{
	return m_Pimpl->GetVolume(activeSoundId);
}

void SdlSoundSystem::SetVolume(ActiveSoundID activeSoundId, float volume)
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

void SdlSoundSystem::SetVolumeAllSounds(float volume)
{
	m_Pimpl->SetVolumeAllSounds(volume);
}

AudioChunk* SdlSoundSystem::GetAudioChunk(SoundID soundId)
{
	return m_Pimpl->GetChunk(soundId);
}


#pragma endregion


#pragma region SdlSoundSystem | PIMPL


SdlSoundSystem::Impl::Impl()
{

	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		std::cout << "AudioChunk: Failed to Initialize Mixer \n";
		throw std::runtime_error(std::string("Failed to Initialize Mixer: ") + SDL_GetError());
	}


	// Disabled VLD, becauseof MMDevApi.dll leaking (only on my system for some reason)
#if defined(_DEBUG) && __has_include(<vld.h>)
	VLDDisable();
#endif

	int openAudioResult = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

#if defined(_DEBUG) && __has_include(<vld.h>)
	VLDEnable();
#endif


	if (openAudioResult < 0)
	{
		std::cout << "AudioChunk: Failed to OpenAudio \n";
		throw std::runtime_error(std::string("Failed to OpenAudio: ") + SDL_GetError());
	}

	Mix_AllocateChannels(m_NrChannels);

}

SdlSoundSystem::Impl::~Impl()
{
	bae::ServiceLocator::RegisterAudioQueue<bae::NullAudioClip>();

	m_LoadedAudio.clear();

	Mix_HaltChannel(-1);
	Mix_CloseAudio();
	Mix_Quit();

	if (SDL_WasInit(SDL_INIT_AUDIO))
		SDL_QuitSubSystem(SDL_INIT_AUDIO);

}


SoundID SdlSoundSystem::Impl::LoadSound(const std::string& path)
{
	if (!std::filesystem::exists(path))
	{
		std::cerr << "SdlSoundSystem::LoadSound File " << path << " not found" << '\n';
		return SoundID();
	}

	// if already loaded
	if (auto it = m_LoadedSoudIDs.find(path); it != m_LoadedSoudIDs.end())
		return it->second;

	// Set before inserting so the soundId starts from 0, and not from 1
	SoundID soundId{ .ID = static_cast<int>(m_LoadedAudio.size()) };

	// load audio chunk
	m_LoadedSoudIDs.insert(std::pair(path, soundId));
	m_LoadedAudio.insert(std::pair(soundId, std::make_unique<AudioChunk>(path)));

	return soundId;
}


ActiveSoundID SdlSoundSystem::Impl::Play(SoundID soundId, float volume)
{
	auto it = m_LoadedAudio.find(soundId);
	if (it == m_LoadedAudio.end())
	{
		std::cout << "Trying To " << GetFunctionName() << " Sound that doesn't Exist, soundId: " << soundId.ID << '\n';
		return ActiveSoundID();
	}

	auto& audioChunk = it->second;
	if (!audioChunk)
		return ActiveSoundID(-1);

	// this also gives an error if not in the correct initialization order :D
	SoundEventData data
	{
		.Type = SoundEventType::Play,
		.SoundID = soundId,
		.ActiveSoundID = ActiveSoundID{.ID = GetRandomNumber() },
		.Volume = volume,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);

	return data.ActiveSoundID;
}

void SdlSoundSystem::Impl::Stop(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::Stop,
		.ActiveSoundID = activeSoundId,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


void SdlSoundSystem::Impl::Resume(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::Resume,
		.ActiveSoundID = activeSoundId,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::Pause(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::Pause,
		.ActiveSoundID = activeSoundId,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


void SdlSoundSystem::Impl::Mute(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::Mute,
		.ActiveSoundID = activeSoundId,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMute(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::UnMute,
		.ActiveSoundID = activeSoundId,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


bool SdlSoundSystem::Impl::IsLoaded(SoundID soundId)
{
	if (m_LoadedAudio.find(soundId) == m_LoadedAudio.end())
		return false;

	return true;
}

bool SdlSoundSystem::Impl::IsPlaying(ActiveSoundID activeSoundId)
{
	// this is special bc we are sending request and you can't immediately get a response back
	// OR
	// we don't use the audioqueue's thread and get the m_ActiveSound's or something like that, ...
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return false;
	}

	auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
	if (!pAudioClip)
		return false;

	return pAudioClip->IsPlaying();
}


bool SdlSoundSystem::Impl::IsPaused(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return false;
	}


	auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
	if (!pAudioClip)
		return false;

	return pAudioClip->IsPaused();
}

bool SdlSoundSystem::Impl::IsMuted(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return false;
	}


	auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
	if (!pAudioClip)
		return false;

	return pAudioClip->IsMuted();
}


float SdlSoundSystem::Impl::GetVolume(ActiveSoundID activeSoundId)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return -1.f;
	}


	auto pAudioClip = ServiceLocator::GetAudioQueue().GetAudioClip(activeSoundId);
	if (!pAudioClip)
		return -1.f;

	return pAudioClip->GetVolume();
}

void SdlSoundSystem::Impl::SetVolume(ActiveSoundID activeSoundId, float volume)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::SetVolume,
		.ActiveSoundID = activeSoundId,
		.Volume = volume
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}


void SdlSoundSystem::Impl::ResumeAllSounds()
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::ResumeAll,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::PauseAllSounds()
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::PauseAll,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::StopAllSounds()
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::StopAll,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::MuteAllSounds()
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::MuteAll,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::UnMuteAllSounds()
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::UnMuteAll,
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

void SdlSoundSystem::Impl::SetVolumeAllSounds(float volume)
{
	if (m_LoadedAudio.empty())
	{
		std::cout << "Trying To " << GetFunctionName() << " but no sound is loaded" << '\n';
		return;
	}


	SoundEventData data
	{
		.Type = SoundEventType::SetVolumeAll,
		.Volume = volume
	};

	ServiceLocator::GetAudioQueue().SendSoundEvent(data);
}

AudioChunk* SdlSoundSystem::Impl::GetChunk(SoundID soundId)
{
	auto it = m_LoadedAudio.find(soundId);
	if (it == m_LoadedAudio.end())
	{
		std::cout << "Trying To " << GetFunctionName() << " but there are no sounds to UnMute" << '\n';
		return nullptr;
	}

	return it->second.get();
}


#pragma endregion


