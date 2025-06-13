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
    Impl(const std::string& test);
    ~Impl();


    void Play();
    void Stop();
    void Mute();

    bool IsPlaying();
    bool IsLoaded();

    float GetVolume();
    void SetVolume(float volume);


private:
    std::unique_ptr<AudioChunk> m_Chunk{};
    int m_Channel{ -1 };
    float m_Volume{ 1.f };
    bool m_bIsLoaded{ false };
    std::mutex m_Mutex;


};



#pragma region SdlAudioClip | NOT PIMPL


SdlAudioClip::SdlAudioClip(const std::string& path)
{
    m_Pimpl = std::make_unique<Impl>(path);
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

void SdlAudioClip::Mute()
{
    m_Pimpl->Mute();
}


bool SdlAudioClip::IsPlaying()
{
    return m_Pimpl->IsPlaying();
}

bool SdlAudioClip::IsLoaded()
{
    return m_Pimpl->IsLoaded();
}


float SdlAudioClip::GetVolume()
{
    return m_Pimpl->GetVolume();
}

void SdlAudioClip::SetVolume(float volume)
{
    m_Pimpl->SetVolume(volume);
}


#pragma endregion


#pragma region SdlAudioClip | PIMPL


SdlAudioClip::Impl::Impl(const std::string& path)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

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
    std::unique_lock<std::mutex> lock(m_Mutex);

    // this should be RAII, let's see if it works
    /*
    if (m_Chunk)
    {
        Mix_FreeChunk(m_Chunk);
        m_Chunk = nullptr;
    }

    Mix_CloseAudio();
    Mix_Quit();
    */
}


void SdlAudioClip::Impl::Play()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

    if (m_bIsLoaded)
    {
        m_Channel = Mix_PlayChannel(-1, m_Chunk->GetChunk(), 0);
    }

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

void SdlAudioClip::Impl::Mute()
{
    std::unique_lock<std::mutex> lock(m_Mutex);

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


float SdlAudioClip::Impl::GetVolume()
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    return m_Volume;
}

void SdlAudioClip::Impl::SetVolume(float volume)
{
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_Volume = std::clamp(volume, 0.f, 1.f);

    if (m_Chunk && m_Chunk->GetChunk())
        Mix_VolumeChunk(m_Chunk->GetChunk(), static_cast<int>(MIX_MAX_VOLUME * m_Volume));

}


#pragma endregion



