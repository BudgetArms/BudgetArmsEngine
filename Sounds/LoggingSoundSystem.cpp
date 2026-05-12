#include "LoggingSoundSystem.h"

#include <iostream>

#include "Core/HelperFunctions.h"
#include "Sounds/NullSoundSystem.h"


using namespace bae;


LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
    std::cout << FUNCTION_NAME << " ";

    if(!soundSystem)
    {
        std::cout << "NullSoundSystem" << '\n';

        m_RealSoundSystem = std::make_unique<NullSoundSystem>();
    }
    else
    {
        [[maybe_unused]] SoundSystem* baseSoundSystem = soundSystem.get();
        const std::string className                   = typeid(baseSoundSystem).name();
        std::cout << className << '\n';

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


ActiveSoundID LoggingSoundSystem::Play(const SoundID soundId)
{
    const ActiveSoundID activeId = m_RealSoundSystem->Play(soundId);
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << " ActiveSoundId: " << activeId.ID << '\n';
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

void LoggingSoundSystem::Loop(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->Loop(activeSoundId);
}

void LoggingSoundSystem::UnLoop(const ActiveSoundID activeSoundId)
{
    std::cout << FUNCTION_NAME << " ActiveSoundId: " << activeSoundId.ID << '\n';
    m_RealSoundSystem->UnLoop(activeSoundId);
}

void LoggingSoundSystem::StopSounds(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << '\n';
    m_RealSoundSystem->StopSounds(soundId);
}

void LoggingSoundSystem::ResumeSounds(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << '\n';
    m_RealSoundSystem->ResumeSounds(soundId);
}

void LoggingSoundSystem::PauseSounds(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << '\n';
    m_RealSoundSystem->PauseSounds(soundId);
}

void LoggingSoundSystem::MuteSounds(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << '\n';
    m_RealSoundSystem->MuteSounds(soundId);
}

void LoggingSoundSystem::UnMuteSounds(const SoundID soundId)
{
    std::cout << FUNCTION_NAME << " SoundId: " << soundId.ID << '\n';
    m_RealSoundSystem->UnMuteSounds(soundId);
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

