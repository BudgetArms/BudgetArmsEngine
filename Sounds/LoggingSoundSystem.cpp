#include "LoggingSoundSystem.hpp"

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"
#include "Sounds/NullSoundSystem.hpp"


using namespace bae;


LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
    if(!soundSystem)
    {
        Logger::LogNormal("Using NullSoundSystem");
        m_RealSoundSystem = std::make_unique<NullSoundSystem>();
    }
    else
    {
        [[maybe_unused]] SoundSystem* baseSoundSystem = soundSystem.get();
        const std::string className                   = typeid(*baseSoundSystem).name();
        Logger::LogNormal("Using " + className);

        m_RealSoundSystem = std::move(soundSystem);
    }
}

LoggingSoundSystem::~LoggingSoundSystem()
{
    Logger::LogNormal("");
}


SoundID LoggingSoundSystem::LoadSound(const std::string& path)
{
    Logger::LogNormal("Path: " + path);
    const SoundID soundID = m_RealSoundSystem->LoadSound(path);
    if(soundID.ID == -1)
    {
        Logger::LogWarning("Failed to load sound, Path: " + path);
    }

    return soundID;
}


ActiveSoundID LoggingSoundSystem::Play(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));

    const ActiveSoundID activeId = m_RealSoundSystem->Play(soundId);
    if(activeId.ID == -1)
    {
        Logger::LogWarning("Failed to play sound, SoundID: " + std::to_string(soundId.ID) + " ActiveSoundId: "
            + std::to_string(activeId.ID));
    }

    return activeId;
}

void LoggingSoundSystem::Stop(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->Stop(activeSoundId);
}


void LoggingSoundSystem::Resume(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->Resume(activeSoundId);
}

void LoggingSoundSystem::Pause(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->Pause(activeSoundId);
}


void LoggingSoundSystem::Mute(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->Mute(activeSoundId);
}

void LoggingSoundSystem::UnMute(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->UnMute(activeSoundId);
}

void LoggingSoundSystem::Loop(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->Loop(activeSoundId);
}

void LoggingSoundSystem::UnLoop(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->UnLoop(activeSoundId);
}

void LoggingSoundSystem::StopSounds(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));
    m_RealSoundSystem->StopSounds(soundId);
}

void LoggingSoundSystem::ResumeSounds(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));
    m_RealSoundSystem->ResumeSounds(soundId);
}

void LoggingSoundSystem::PauseSounds(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));
    m_RealSoundSystem->PauseSounds(soundId);
}

void LoggingSoundSystem::MuteSounds(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));
    m_RealSoundSystem->MuteSounds(soundId);
}

void LoggingSoundSystem::UnMuteSounds(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));
    m_RealSoundSystem->UnMuteSounds(soundId);
}


void LoggingSoundSystem::ResumeAllSounds()
{
    Logger::LogNormal("");
    m_RealSoundSystem->ResumeAllSounds();
}

void LoggingSoundSystem::PauseAllSounds()
{
    Logger::LogNormal("");
    m_RealSoundSystem->PauseAllSounds();
}

void LoggingSoundSystem::StopAllSounds()
{
    Logger::LogNormal("");
    m_RealSoundSystem->StopAllSounds();
}

void LoggingSoundSystem::MuteAllSounds()
{
    Logger::LogNormal("");
    m_RealSoundSystem->MuteAllSounds();
}

void LoggingSoundSystem::UnMuteAllSounds()
{
    Logger::LogNormal("");
    m_RealSoundSystem->UnMuteAllSounds();
}

void LoggingSoundSystem::SetVolumeAllSounds(const float volume)
{
    Logger::LogNormal("Volume: " + std::to_string(volume));
    m_RealSoundSystem->SetVolumeAllSounds(volume);
}


bool LoggingSoundSystem::IsLoaded(const SoundID soundId)
{
    Logger::LogNormal("SoundId: " + std::to_string(soundId.ID));
    return m_RealSoundSystem->IsLoaded(soundId);
}

bool LoggingSoundSystem::IsPlaying(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    return m_RealSoundSystem->IsPlaying(activeSoundId);
}


bool LoggingSoundSystem::IsPaused(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    return m_RealSoundSystem->IsPaused(activeSoundId);
}

bool LoggingSoundSystem::IsMuted(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    return m_RealSoundSystem->IsMuted(activeSoundId);
}


float LoggingSoundSystem::GetVolume(const ActiveSoundID activeSoundId)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    return m_RealSoundSystem->GetVolume(activeSoundId);
}

void LoggingSoundSystem::SetVolume(const ActiveSoundID activeSoundId, const float volume)
{
    Logger::LogNormal("ActiveSoundId: " + std::to_string(activeSoundId.ID));
    m_RealSoundSystem->SetVolume(activeSoundId, volume);
}

