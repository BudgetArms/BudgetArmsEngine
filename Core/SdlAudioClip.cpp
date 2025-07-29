#include "SdlAudioClip.h"

#include <iostream>
#include <mutex>
#include <algorithm>


#include <SDL_mixer.h>

#include "Core/ServiceLocator.h"
#include "Wrappers/AudioChunk.h"
#include "Core/SoundSystem.h"

#include "Core/HelperFunctions.h"


using namespace bae;


class SdlAudioClip::Impl
{
public:
	Impl();
	Impl(ActiveSoundID activeSoundId, SoundID soundId);
	~Impl();


	bool Play();
	void Stop();

	void Resume();
	void Pause();

	void Mute();
	void UnMute();


	bool IsPlaying() const;

	bool IsPaused() const;
	bool IsMuted() const;


	float GetVolume() const;
	void SetVolume(float volume);

	SoundID GetSoundId();
	ActiveSoundID GetActiveSoundId();
	int GetChannel();



	std::mutex m_Mutex;

private:
	SoundID m_SoundId{};
	ActiveSoundID m_ActiveSoundID{};
	int m_Channel{ -1 };
	float m_Volume{ 1.f };
	bool m_Test{ false };
	bool m_bIsPaused{ false };
	bool m_bIsMuted{ false };


};



#pragma region SdlAudioClip | NOT PIMPL

//SdlAudioClip::SdlAudioClip()
//{
//
//}

SdlAudioClip::SdlAudioClip(ActiveSoundID activeId, SoundID soundId)
{
	m_Pimpl = std::make_unique<Impl>(activeId, soundId);
}

SdlAudioClip::~SdlAudioClip()
{
}


bool SdlAudioClip::Play()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->Play();
}

void SdlAudioClip::Stop()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	m_Pimpl->Stop();
}


void SdlAudioClip::Resume()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	m_Pimpl->Resume();
}

void SdlAudioClip::Pause()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	m_Pimpl->Pause();
}


void SdlAudioClip::Mute()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	m_Pimpl->Mute();
}

void SdlAudioClip::UnMute()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	m_Pimpl->UnMute();
}


bool SdlAudioClip::IsPlaying() const
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->IsPlaying();
}


bool SdlAudioClip::IsPaused() const
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->IsPaused();
}

bool SdlAudioClip::IsMuted() const
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->IsMuted();
}



float SdlAudioClip::GetVolume() const
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->GetVolume();
}

void SdlAudioClip::SetVolume(float volume)
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	m_Pimpl->SetVolume(volume);
}


SoundID SdlAudioClip::GetSoundId()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->GetSoundId();
}

ActiveSoundID SdlAudioClip::GetActiveSoundId()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->GetActiveSoundId();
}

int SdlAudioClip::GetChannel()
{
	std::lock_guard<std::mutex> lock{ m_Pimpl->m_Mutex };
	return m_Pimpl->GetChannel();
}


#pragma endregion


#pragma region SdlAudioClip | PIMPL


SdlAudioClip::Impl::Impl(ActiveSoundID id, SoundID soundId)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	m_ActiveSoundID = id;
	m_SoundId = soundId;


	m_Channel = -1;
	m_Volume = 1.0f;
	SetVolume(m_Volume);

}

SdlAudioClip::Impl::~Impl()
{
}


bool SdlAudioClip::Impl::Play()
{
	m_Channel = Mix_PlayChannel(-1, ServiceLocator::GetSoundSystem().GetAudioChunk(m_SoundId)->GetChunk(), 0);

	if (m_Channel == -1)
	{
		std::cout << GetFunctionName() << " Channels are full!!!\n";
		return false;
	}

	if (m_bIsMuted)
		Mute();

	return true;
}

void SdlAudioClip::Impl::Stop()
{

	if (IsPlaying())
	{
		if (m_Channel == -1)
			return;

		Mix_HaltChannel(m_Channel);
		m_Channel = -1;
	}
}


void SdlAudioClip::Impl::Resume()
{
	if (m_Channel == -1)
		return;

	if (Mix_Paused(m_Channel))
	{
		Mix_Resume(m_Channel);
		m_bIsPaused = false;
	}

}

void SdlAudioClip::Impl::Pause()
{
	if (m_Channel == -1)
		return;

	if (!Mix_Paused(m_Channel))
	{
		Mix_Pause(m_Channel);
		m_bIsPaused = true;
	}
}


void SdlAudioClip::Impl::Mute()
{
	if (m_Channel == -1)
		return;

	m_bIsMuted = true;
	Mix_Volume(m_Channel, 0); // can't use SetVolume, bc it also changes m_Volume
}

void SdlAudioClip::Impl::UnMute()
{
	if (m_Channel == -1)
		return;

	m_bIsMuted = false;
	SetVolume(m_Volume);
}


bool SdlAudioClip::Impl::IsPlaying() const
{
	if (m_Channel == -1)
		return false;

	return Mix_Playing(m_Channel);
}


bool SdlAudioClip::Impl::IsPaused() const
{
	return m_bIsPaused;
}

bool SdlAudioClip::Impl::IsMuted() const
{
	return m_bIsMuted;
}


float SdlAudioClip::Impl::GetVolume() const
{
	return m_Volume;
}

void SdlAudioClip::Impl::SetVolume(float volume)
{
	if (m_Channel == -1)
		return;

	m_Volume = std::clamp(volume, 0.f, 1.f);

	if (!m_bIsMuted)
		Mix_Volume(m_Channel, static_cast<int>(MIX_MAX_VOLUME * m_Volume));
}

SoundID SdlAudioClip::Impl::GetSoundId()
{
	return m_SoundId;
}

ActiveSoundID SdlAudioClip::Impl::GetActiveSoundId()
{
	return m_ActiveSoundID;
}

int SdlAudioClip::Impl::GetChannel()
{
	return m_Channel;
}


#pragma endregion



