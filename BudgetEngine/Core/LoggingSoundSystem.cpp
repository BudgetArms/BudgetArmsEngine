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

void LoggingSoundSystem::Mute(int soundId)
{
    std::cout << "LoggingSoundSystem: Mute Sound: " << soundId << '\n';
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


