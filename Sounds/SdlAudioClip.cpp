#include "SdlAudioClip.h"

#include <algorithm>
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
    void Stop() const;

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
    static void SDLCALL TrackFinishedCallBack(void* isStoppedData, MIX_Track*);

    SoundID m_SoundId{};
    ActiveSoundID m_ActiveSoundID{};
    AudioTrack m_Track{};

    float m_Volume{ 1.f };
    bool m_bIsMuted{ false };
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
};


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

    success = MIX_SetTrackStoppedCallback(m_Track.GetTrack(), TrackFinishedCallBack, &m_bIsStopped);
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
    std::cout << FUNCTION_NAME << " Destroyed AudioClip" << '\n';
}


bool SdlAudioClip::Impl::Play()
{
    auto track         = m_Track.GetTrack();
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

void SdlAudioClip::Impl::Stop() const
{
    MIX_StopTrack(m_Track.GetTrack(), 0);
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


void __cdecl SdlAudioClip::Impl::TrackFinishedCallBack(void* isStoppedData, MIX_Track*)
{
    bool* const isStopped = static_cast<bool*>(isStoppedData);
    *isStopped            = true;
}

#pragma endregion


