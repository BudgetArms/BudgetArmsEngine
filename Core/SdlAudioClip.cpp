#include "SdlAudioClip.h"

#include <iostream>
#include <mutex>
#include <algorithm>

#include <SDL_mixer.h>

#include "Wrappers/AudioChunk.h"


using namespace bae;


class SdlAudioClip::Impl
{
public:
    Impl(const std::string& path, int soundId);
    ~Impl();


    void Play();
    void Stop();

    void Resume();
    void Pause();

    void Mute();
    void UnMute();


    bool IsPlaying();
    bool IsLoaded();

    bool IsPaused();
    bool IsMuted();


    float GetVolume();
    void SetVolume(float volume);

    int GetSoundId();


    std::mutex m_Mutex;


private:
    std::unique_ptr<AudioChunk> m_Chunk{};
    int m_SoundId{};
    int m_Channel{ -1 };
    float m_Volume{ 1.f };
    bool m_bIsLoaded{ false };
    bool m_bIsPaused{ false };
    bool m_bIsMuted{ false };


};



#pragma region SdlAudioClip | NOT PIMPL


SdlAudioClip::SdlAudioClip(const std::string& path, int soundId)
{
    m_Pimpl = std::make_unique<Impl>(path, soundId);
}

SdlAudioClip::~SdlAudioClip()
{
}


void SdlAudioClip::Play()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Play();
}

void SdlAudioClip::Stop()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Stop();
}


void SdlAudioClip::Resume()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Resume();
}

void SdlAudioClip::Pause()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Pause();
}


void SdlAudioClip::Mute()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->Mute();
}

void SdlAudioClip::UnMute()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->UnMute();
}


bool SdlAudioClip::IsPlaying()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsPlaying();
}

bool SdlAudioClip::IsLoaded()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsLoaded();
}


bool SdlAudioClip::IsPaused()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsPaused();
}

bool SdlAudioClip::IsMuted()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->IsMuted();
}



float SdlAudioClip::GetVolume()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->GetVolume();
}

void SdlAudioClip::SetVolume(float volume)
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    m_Pimpl->SetVolume(volume);
}


int SdlAudioClip::GetSoundId()
{
    std::unique_lock<std::mutex> lock{ m_Pimpl->m_Mutex };
    return m_Pimpl->GetSoundId();
}


#pragma endregion


#pragma region SdlAudioClip | PIMPL


SdlAudioClip::Impl::Impl(const std::string& path, int soundId)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    m_SoundId = soundId;

    if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        std::cout << "SdlAudioClip sdl_mixer could not initialize SDL_mixer \n";
        m_bIsLoaded = false;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SdlAudioClip sdl_mixer could not openaudio or something \n";
        m_bIsLoaded = false;
        return;
    }

    m_Chunk = std::make_unique<AudioChunk>(path);
    if (!m_Chunk)
    {
        std::cout << "Chunk failed\n";
        m_bIsLoaded = false;
        return;
    }

    m_bIsLoaded = true;
    m_Channel = -1;
    m_Volume = 1.0f;
    Mix_VolumeChunk(m_Chunk->GetChunk(), static_cast<int>(MIX_MAX_VOLUME * m_Volume));


}

SdlAudioClip::Impl::~Impl()
{
}


void SdlAudioClip::Impl::Play()
{
    if (m_bIsLoaded)
        m_Channel = Mix_PlayChannel(-1, m_Chunk->GetChunk(), 0);

    if (m_bIsMuted)
        Mute();

}

void SdlAudioClip::Impl::Stop()
{
    if (IsPlaying())
    {
        Mix_HaltChannel(m_Channel);
        m_Channel = -1;
    }
}


void SdlAudioClip::Impl::Resume()
{
    if (m_Channel != -1)
        if (Mix_Paused(m_Channel))
        {
            Mix_Resume(m_Channel);
            m_bIsPaused = false;
        }

}

void SdlAudioClip::Impl::Pause()
{
    if (m_Channel != -1)
        if (!Mix_Paused(m_Channel))
        {
            Mix_Pause(m_Channel);
            m_bIsPaused = true;
        }

}


void SdlAudioClip::Impl::Mute()
{
    Mix_VolumeChunk(m_Chunk->GetChunk(), 0);
    m_bIsMuted = true;
}

void SdlAudioClip::Impl::UnMute()
{
    if (!m_bIsMuted)
        return;

    SetVolume(m_Volume);
    m_bIsMuted = false;
}


bool SdlAudioClip::Impl::IsPlaying()
{
    return (m_Channel != -1) && (Mix_Playing(m_Channel) != 0);
}

bool SdlAudioClip::Impl::IsLoaded()
{
    return m_bIsLoaded;
}


bool SdlAudioClip::Impl::IsPaused()
{
    return m_bIsPaused;
}

bool SdlAudioClip::Impl::IsMuted()
{
    return m_bIsMuted;
}


float SdlAudioClip::Impl::GetVolume()
{
    return m_Volume;
}

void SdlAudioClip::Impl::SetVolume(float volume)
{
    m_Volume = std::clamp(volume, 0.f, 1.f);
    Mix_VolumeChunk(m_Chunk->GetChunk(), static_cast<int>(MIX_MAX_VOLUME * m_Volume));
}


int SdlAudioClip::Impl::GetSoundId()
{
    return m_SoundId;
}


#pragma endregion



