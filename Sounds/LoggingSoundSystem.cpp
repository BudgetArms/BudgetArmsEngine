#include "LoggingSoundSystem.h"

#include <iostream>

#include "Core/HelperFunctions.h"
#include "Sounds/NullSoundSystem.h"


using namespace bae;


LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
    std::cout << FUNCTION_NAME << '\n';
    if(!soundSystem)
    {
        m_RealSoundSystem = std::make_unique<NullSoundSystem>();
    }
    else
    {
        m_RealSoundSystem = std::move(soundSystem);
    }
}

LoggingSoundSystem::~LoggingSoundSystem()
{
    std::cout << FUNCTION_NAME << '\n';
}


SoundID LoggingSoundSystem::LoadSound(const std::string& path)
{
    std::cout << "LoggingSoundSystem: Loading Sound, path: " << path << '\n';
    return m_RealSoundSystem->LoadSound(path);
}


ActiveSoundID LoggingSoundSystem::Play(const SoundID soundId, const float volume)
{
    const ActiveSoundID activeId = m_RealSoundSystem->Play(soundId, volume);
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << " ActiveSoundId: " << activeId.ID << ", volume: " <<
            volume << '\n';
    return activeId;
}

void LoggingSoundSystem::Stop(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->Stop(activeSoundId);
}


void LoggingSoundSystem::Resume(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->Resume(activeSoundId);
}

void LoggingSoundSystem::Pause(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->Pause(activeSoundId);
}


void LoggingSoundSystem::Mute(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->Mute(activeSoundId);
}

void LoggingSoundSystem::UnMute(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->UnMute(activeSoundId);
}


bool LoggingSoundSystem::IsLoaded(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << " soundId: " << soundId.ID << '\n';
    return m_RealSoundSystem->IsLoaded(soundId);
}

bool LoggingSoundSystem::IsPlaying(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    return m_RealSoundSystem->IsPlaying(activeSoundId);
}


bool LoggingSoundSystem::IsPaused(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    return m_RealSoundSystem->IsPaused(activeSoundId);
}

bool LoggingSoundSystem::IsMuted(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    return m_RealSoundSystem->IsMuted(activeSoundId);
}


float LoggingSoundSystem::GetVolume(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    return m_RealSoundSystem->GetVolume(activeSoundId);
}

void LoggingSoundSystem::SetVolume(const ActiveSoundID activeSoundId, const float volume)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->SetVolume(activeSoundId, volume);
}


void LoggingSoundSystem::ResumeAllSounds()
{
    std::cout << FUNCTION_NAME << '\n';
    m_RealSoundSystem->ResumeAllSounds();
}

void LoggingSoundSystem::PauseAllSounds()
{
    std::cout << FUNCTION_NAME << '\n';
    m_RealSoundSystem->PauseAllSounds();
}

void LoggingSoundSystem::StopAllSounds()
{
    std::cout << FUNCTION_NAME << '\n';
    m_RealSoundSystem->StopAllSounds();
}

void LoggingSoundSystem::MuteAllSounds()
{
    std::cout << FUNCTION_NAME << '\n';
    m_RealSoundSystem->MuteAllSounds();
}

void LoggingSoundSystem::UnMuteAllSounds()
{
    std::cout << FUNCTION_NAME << '\n';
    m_RealSoundSystem->UnMuteAllSounds();
}

void LoggingSoundSystem::SetVolumeAllSounds(const float volume)
{
    std::cout << FUNCTION_NAME << '\n';
    m_RealSoundSystem->SetVolumeAllSounds(volume);
}

Audio* LoggingSoundSystem::GetAudio(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << '\n';
    return m_RealSoundSystem->GetAudio(soundId);
}

MIX_Mixer* LoggingSoundSystem::GetMixer()
{
    std::cout << FUNCTION_NAME << '\n';
    return m_RealSoundSystem->GetMixer();
}

