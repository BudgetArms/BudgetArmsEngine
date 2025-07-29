#include "LoggingSoundSystem.h"

#include <iostream>

#include "Core/HelperFunctions.h"


using namespace bae;


LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
	std::cout << GetFunctionName() << '\n';
	if (!soundSystem)
		m_RealSoundSystem = std::make_unique<NullSoundSystem>();
	else
		m_RealSoundSystem = std::move(soundSystem);
}

LoggingSoundSystem::~LoggingSoundSystem()
{
	std::cout << GetFunctionName() << '\n';
}


SoundID LoggingSoundSystem::LoadSound(const std::string& path)
{
	std::cout << "LoggingSoundSystem: Loading Sound, path: " << path << '\n';
	return m_RealSoundSystem->LoadSound(path);
}


ActiveSoundID LoggingSoundSystem::Play(SoundID soundId, float volume)
{
	ActiveSoundID asID = m_RealSoundSystem->Play(soundId, volume);
	std::cout << GetFunctionName() << " SoundId: " << soundId.ID << " ActiveSoundId: " << asID.ID << ", volume: " << volume << '\n';
	return asID;
}

void LoggingSoundSystem::Stop(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	m_RealSoundSystem->Stop(activeSoundId);
}


void LoggingSoundSystem::Resume(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	m_RealSoundSystem->Resume(activeSoundId);
}

void LoggingSoundSystem::Pause(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	m_RealSoundSystem->Pause(activeSoundId);
}


void LoggingSoundSystem::Mute(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	m_RealSoundSystem->Mute(activeSoundId);
}

void LoggingSoundSystem::UnMute(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	m_RealSoundSystem->Mute(activeSoundId);
}


bool LoggingSoundSystem::IsLoaded(SoundID soundId)
{
	std::cout << GetFunctionName() << " soundId: " << soundId.ID << '\n';
	return m_RealSoundSystem->IsLoaded(soundId);
}

bool LoggingSoundSystem::IsPlaying(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	return m_RealSoundSystem->IsPlaying(activeSoundId);
}


bool LoggingSoundSystem::IsPaused(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	return m_RealSoundSystem->IsPaused(activeSoundId);
}

bool LoggingSoundSystem::IsMuted(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	return m_RealSoundSystem->IsMuted(activeSoundId);
}


float LoggingSoundSystem::GetVolume(ActiveSoundID activeSoundId)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	return m_RealSoundSystem->GetVolume(activeSoundId);
}

void LoggingSoundSystem::SetVolume(ActiveSoundID activeSoundId, float volume)
{
	std::cout << GetFunctionName() << " ActiveSoundId: " << activeSoundId.ID << '\n';
	m_RealSoundSystem->SetVolume(activeSoundId, volume);
}


void LoggingSoundSystem::ResumeAllSounds()
{
	std::cout << GetFunctionName() << '\n';
	m_RealSoundSystem->ResumeAllSounds();
}

void LoggingSoundSystem::PauseAllSounds()
{
	std::cout << GetFunctionName() << '\n';
	m_RealSoundSystem->PauseAllSounds();
}

void LoggingSoundSystem::StopAllSounds()
{
	std::cout << GetFunctionName() << '\n';
	m_RealSoundSystem->StopAllSounds();
}

void LoggingSoundSystem::MuteAllSounds()
{
	std::cout << GetFunctionName() << '\n';
	m_RealSoundSystem->MuteAllSounds();
}

void LoggingSoundSystem::UnMuteAllSounds()
{
	std::cout << GetFunctionName() << '\n';
	m_RealSoundSystem->UnMuteAllSounds();
}

void LoggingSoundSystem::SetVolumeAllSounds(float volume)
{
	std::cout << GetFunctionName() << '\n';
	m_RealSoundSystem->SetVolumeAllSounds(volume);
}

AudioChunk* LoggingSoundSystem::GetAudioChunk(SoundID soundId)
{
	std::cout << GetFunctionName() << '\n';
	return m_RealSoundSystem->GetAudioChunk(soundId);
}


