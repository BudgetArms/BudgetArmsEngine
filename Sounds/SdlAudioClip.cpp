#include "SdlAudioClip.h"

#include <algorithm>
#include <iostream>
#include <mutex>

#include <SDL3_mixer/SDL_mixer.h>

#include "Core/ServiceLocator.h"
#include "Sounds/SoundSystem.h"


using namespace bae;


class SdlAudioClip::Impl
{
public:
    Impl(ActiveSoundID activeSoundId, SoundID soundId);
    ~Impl();


    bool Play();
    void Stop();

    void Loop();
    void UnLoop();

    void Resume() const;
    void Pause() const;

    void Mute();
    void UnMute();


    [[nodiscard]] bool IsPlaying() const;

    [[nodiscard]] bool IsPaused() const;
    [[nodiscard]] bool IsMuted() const;
    [[nodiscard]] bool IsStopped() const;


    [[nodiscard]] float GetVolume() const;
    void SetVolume(float volume);

    [[nodiscard]] SoundID GetSoundId() const;
    [[nodiscard]] ActiveSoundID GetActiveSoundId() const;
    [[nodiscard]] AudioTrack& GetAudioTrack();


    std::mutex m_Mutex;

private:
    static void SDLCALL TrackFinishedCallBack(void* data, MIX_Track*);

    SoundID m_SoundId{ -1 };
    ActiveSoundID m_ActiveSoundID{ -1 };
    AudioTrack m_Track{};

    float m_Volume{ 1.f };
    bool m_bIsMuted{ false };
    bool m_bIsLooped{ false };
    bool m_bIsStopped{ false };
};


#pragma region SdlAudioClip | NOT PIMPL


SdlAudioClip::SdlAudioClip(ActiveSoundID activeId, SoundID soundId) :
    AudioClip(activeId, soundId)
{
    m_Pimpl = std::make_unique<Impl>(activeId, soundId);
}

SdlAudioClip::~SdlAudioClip()
{
    std::cout << FUNCTION_NAME << '\n';
}


bool SdlAudioClip::Play()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->Play();
}

void SdlAudioClip::Stop()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Stop();
}

void SdlAudioClip::Resume()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Resume();
}

void SdlAudioClip::Pause()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Pause();
}


void SdlAudioClip::Mute()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Mute();
}

void SdlAudioClip::UnMute()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->UnMute();
}


void SdlAudioClip::Loop()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Loop();
}

void SdlAudioClip::UnLoop()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->UnLoop();
}


bool SdlAudioClip::IsPlaying() const
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsPlaying();
}


bool SdlAudioClip::IsPaused() const
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsPaused();
}

bool SdlAudioClip::IsMuted() const
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsMuted();
}

bool SdlAudioClip::IsStopped() const
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsStopped();
}


float SdlAudioClip::GetVolume() const
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->GetVolume();
}

void SdlAudioClip::SetVolume(const float volume)
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    m_Pimpl->SetVolume(volume);
}


SoundID SdlAudioClip::GetSoundId()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->GetSoundId();
}

ActiveSoundID SdlAudioClip::GetActiveSoundId()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->GetActiveSoundId();
}

AudioTrack& SdlAudioClip::GetAudioTrack()
{
    std::lock_guard lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->GetAudioTrack();
}


#pragma endregion


#pragma region SdlAudioClip | PIMPL


SdlAudioClip::Impl::Impl(const ActiveSoundID activeSoundId, const SoundID soundId) :
    m_Track{ AudioTrack() }
{
    std::lock_guard lock(m_Mutex);

    m_ActiveSoundID = activeSoundId;
    m_SoundId       = soundId;

    const Audio* audio = ServiceLocator::GetSoundSystem().GetAudio(soundId);
    if(!audio)
    {
        std::cout << FUNCTION_NAME << " Failed to get valid audio from SoundID: " << soundId.ID << '\n';
        return;
    }

    bool success = MIX_SetTrackAudio(m_Track.GetTrack(), audio->GetAudio());
    if(!success)
    {
        std::cout << FUNCTION_NAME << " Failed to set Audio on Track, Error:" << SDL_GetError() << '\n';
        return;
    }


    success = MIX_SetTrackStoppedCallback(m_Track.GetTrack(), TrackFinishedCallBack, this);
    if(!success)
    {
        std::cout << FUNCTION_NAME << " Failed to create Track Stopped Callback, Error:" << SDL_GetError() << '\n';
        return;
    }

    m_Volume = 1.0f;
    SetVolume(m_Volume);
}

SdlAudioClip::Impl::~Impl()
{
    std::cout << FUNCTION_NAME << " Destroyed AudioClip, ActiveSoundID: " << m_ActiveSoundID.ID << '\n';
}


bool SdlAudioClip::Impl::Play()
{
    const auto track   = m_Track.GetTrack();
    const bool success = MIX_PlayTrack(track, 0);
    if(!success)
    {
        std::cout << FUNCTION_NAME << " Failed to play sound, Error: " << SDL_GetError() << '\n';
        return false;
    }

    if(m_bIsMuted)
    {
        Mute();
    }

    return true;
}

void SdlAudioClip::Impl::Stop()
{
    m_bIsStopped = true;

    // TODO: remove the line below to check if it still works
    // This shouldn't be necessary due to AudioQueue
    MIX_StopTrack(m_Track.GetTrack(), 0);
}

void SdlAudioClip::Impl::Loop()
{
    m_bIsLooped = true;
}

void SdlAudioClip::Impl::UnLoop()
{
    m_bIsLooped = false;
}


void SdlAudioClip::Impl::Resume() const
{
    MIX_ResumeTrack(m_Track.GetTrack());
}

void SdlAudioClip::Impl::Pause() const
{
    MIX_PauseTrack(m_Track.GetTrack());
}


void SdlAudioClip::Impl::Mute()
{
    m_bIsMuted = true;
    MIX_SetTrackGain(m_Track.GetTrack(), 0.0f);
}

void SdlAudioClip::Impl::UnMute()
{
    m_bIsMuted = false;
    MIX_SetTrackGain(m_Track.GetTrack(), m_Volume);
}


bool SdlAudioClip::Impl::IsPlaying() const
{
    return MIX_TrackPlaying(m_Track.GetTrack());
}


bool SdlAudioClip::Impl::IsPaused() const
{
    return MIX_TrackPaused(m_Track.GetTrack());
}

bool SdlAudioClip::Impl::IsMuted() const
{
    return m_bIsMuted;
}

bool SdlAudioClip::Impl::IsStopped() const
{
    return m_bIsStopped;
}


float SdlAudioClip::Impl::GetVolume() const
{
    return m_Volume;
}

void SdlAudioClip::Impl::SetVolume(const float volume)
{
    m_Volume = std::clamp(volume, 0.f, 1.f);

    if(!IsMuted())
    {
        MIX_SetTrackGain(m_Track.GetTrack(), m_Volume);
    }
}

SoundID SdlAudioClip::Impl::GetSoundId() const
{
    return m_SoundId;
}

ActiveSoundID SdlAudioClip::Impl::GetActiveSoundId() const
{
    return m_ActiveSoundID;
}

AudioTrack& SdlAudioClip::Impl::GetAudioTrack()
{
    return m_Track;
}


void __cdecl SdlAudioClip::Impl::TrackFinishedCallBack(void* data, MIX_Track*)
{
    const auto pImpl = static_cast<Impl*>(data);
    if(!pImpl)
    {
        std::cout << FUNCTION_NAME << " Failed! data couldn't be casted to Impl*" << '\n';
        return;
    }

    if(pImpl->m_bIsLooped)
    {
        // This might seem stupid to do, since we could set the properties during MIX_PlayTrack
        // to make it looped, but I want to have the ability to loop/unloop a track any time I want.
        const bool success = MIX_PlayTrack(pImpl->m_Track.GetTrack(), 0);
        if(!success)
        {
            std::cout << FUNCTION_NAME << " Failed to loop/replay the same track, Error: " << SDL_GetError() << '\n';
        }

        return;
    }

    pImpl->m_bIsStopped = true;
}

#pragma endregion


