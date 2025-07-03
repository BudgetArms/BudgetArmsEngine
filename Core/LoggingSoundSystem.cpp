#include "LoggingSoundSystem.h"

#include <iostream>


using namespace bae;


LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
    std::cout << "LoggingSoundSystem\n";
    if (!soundSystem)
        m_RealSoundSystem = std::make_unique<NullSoundSystem>();
    else
        m_RealSoundSystem = std::move(soundSystem);
}

LoggingSoundSystem::~LoggingSoundSystem()
{
    std::cout << "~LoggingSoundSystem\n";
}


int LoggingSoundSystem::LoadSound(const std::string& path)
{
    std::cout << "LoggingSoundSystem: Loading Sound, path: " << path << '\n';
    return m_RealSoundSystem->LoadSound(path);
}


void LoggingSoundSystem::Play(int soundId, float volume)
{
    std::cout << "LoggingSoundSystem: Play Sound: " << soundId << ", value: " << volume << '\n';
    m_RealSoundSystem->Play(soundId, volume);
}

void LoggingSoundSystem::Stop(int soundId)
{
    std::cout << "LoggingSoundSystem: Stop Sound: " << soundId << '\n';
    m_RealSoundSystem->Stop(soundId);
}


void LoggingSoundSystem::Resume(int soundId)
{
    std::cout << "LoggingSoundSystem: Resume Sound: " << soundId << '\n';
    m_RealSoundSystem->Resume(soundId);
}

void LoggingSoundSystem::Pause(int soundId)
{
    std::cout << "LoggingSoundSystem: Pause Sound: " << soundId << '\n';
    m_RealSoundSystem->Pause(soundId);
}


void LoggingSoundSystem::Mute(int soundId)
{
    std::cout << "LoggingSoundSystem: Mute Sound: " << soundId << '\n';
    m_RealSoundSystem->Mute(soundId);
}

void LoggingSoundSystem::UnMute(int soundId)
{
    std::cout << "LoggingSoundSystem: UnMute Sound: " << soundId << '\n';
    m_RealSoundSystem->Mute(soundId);
}


bool LoggingSoundSystem::IsLoaded(int soundId)
{
    std::cout << "LoggingSoundSystem: IsLoaded, sondId: " << soundId << '\n';
    return m_RealSoundSystem->IsLoaded(soundId);
}

bool LoggingSoundSystem::IsPlaying(int soundId)
{
    std::cout << "LoggingSoundSystem: IsPlaying, sondId: " << soundId << '\n';
    return m_RealSoundSystem->IsPlaying(soundId);
}


bool LoggingSoundSystem::IsPaused(int soundId)
{
    std::cout << "LoggingSoundSystem: IsPaused, sondId: " << soundId << '\n';
    return m_RealSoundSystem->IsPaused(soundId);
}

bool LoggingSoundSystem::IsMuted(int soundId)
{
    std::cout << "LoggingSoundSystem: IsMuted, sondId: " << soundId << '\n';
    return m_RealSoundSystem->IsMuted(soundId);
}


void LoggingSoundSystem::ResumeAllSounds()
{
    std::cout << "LoggingSoundSystem: ResumeAllSounds" << '\n';
    m_RealSoundSystem->ResumeAllSounds();
}

void LoggingSoundSystem::PauseAllSounds()
{
    std::cout << "LoggingSoundSystem: PauseAllSounds" << '\n';
    m_RealSoundSystem->PauseAllSounds();
}

void LoggingSoundSystem::StopAllSounds()
{
    std::cout << "LoggingSoundSystem: StopAllSounds" << '\n';
    m_RealSoundSystem->StopAllSounds();
}

void LoggingSoundSystem::MuteAllSounds()
{
    std::cout << "LoggingSoundSystem: MuteAllSounds" << '\n';
    m_RealSoundSystem->MuteAllSounds();
}

void LoggingSoundSystem::UnMuteAllSounds()
{
    std::cout << "LoggingSoundSystem: UnMuteAllSounds" << '\n';
    m_RealSoundSystem->UnMuteAllSounds();
}


