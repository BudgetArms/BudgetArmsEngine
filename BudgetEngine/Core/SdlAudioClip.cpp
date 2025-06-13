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


private:
    std::unique_ptr<AudioChunk> m_Chunk{};
    int m_SoundId{};
    int m_Channel{ -1 };
    float m_Volume{ 1.f };
    bool m_bIsLoaded{ false };
    bool m_bIsPaused{ false };
    bool m_bIsMuted{ false };
    std::mutex m_Mutex;


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
    m_Pimpl->Play();
}

void SdlAudioClip::Stop()
{
    m_Pimpl->Stop();
}


void SdlAudioClip::Resume()
{
    m_Pimpl->Resume();
}

void SdlAudioClip::Pause()
{
    m_Pimpl->Pause();
}


void SdlAudioClip::Mute()
{
    m_Pimpl->Mute();
}

void SdlAudioClip::UnMute()
{
    m_Pimpl->UnMute();
}


bool SdlAudioClip::IsPlaying()
{
    return m_Pimpl->IsPlaying();
}

bool SdlAudioClip::IsLoaded()
{
    return m_Pimpl->IsLoaded();
}


bool SdlAudioClip::IsPaused()
{
    return m_Pimpl->IsPaused();
}

bool SdlAudioClip::IsMuted()
{
    return m_Pimpl->IsMuted();
}



float SdlAudioClip::GetVolume()
{
    return m_Pimpl->GetVolume();
}

void SdlAudioClip::SetVolume(float volume)
{
    m_Pimpl->SetVolume(volume);
}


int SdlAudioClip::GetSoundId()
{
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
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (m_bIsLoaded)
        m_Channel = Mix_PlayChannel(-1, m_Chunk->GetChunk(), 0);

}

void SdlAudioClip::Impl::Stop()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (IsPlaying())
    {
        Mix_HaltChannel(m_Channel);
        m_Channel = -1;
    }
}


void SdlAudioClip::Impl::Resume()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (m_Channel != -1)
        if (Mix_Paused(m_Channel))
        {
            Mix_Resume(m_Channel);
            m_bIsPaused = false;
        }

}

void SdlAudioClip::Impl::Pause()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (m_Channel != -1)
        if (!Mix_Paused(m_Channel))
        {
            Mix_Pause(m_Channel);
            m_bIsPaused = true;
        }

}


void SdlAudioClip::Impl::Mute()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (m_bIsMuted)
        return;

    Mix_VolumeChunk(m_Chunk->GetChunk(), 0);
    m_bIsMuted = true;
}

void SdlAudioClip::Impl::UnMute()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (!m_bIsMuted)
        return;

    //Mix_VolumeChunk(m_Chunk->GetChunk(), m_Volume);
    SetVolume(m_Volume);
    m_bIsMuted = false;
}


bool SdlAudioClip::Impl::IsPlaying()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return (m_Channel != -1) && (Mix_Playing(m_Channel) != 0);
}

bool SdlAudioClip::Impl::IsLoaded()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_bIsLoaded;
}


bool SdlAudioClip::Impl::IsPaused()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_bIsPaused;
}

bool SdlAudioClip::Impl::IsMuted()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_bIsMuted;
}


float SdlAudioClip::Impl::GetVolume()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_Volume;
}

void SdlAudioClip::Impl::SetVolume(float volume)
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_Volume = std::clamp(volume, 0.f, 1.f);
    Mix_VolumeChunk(m_Chunk->GetChunk(), static_cast<int>(MIX_MAX_VOLUME * m_Volume));
}


int SdlAudioClip::Impl::GetSoundId()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_SoundId;
}


#pragma endregion



