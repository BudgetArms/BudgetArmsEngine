#pragma once

#include <string>


namespace bae
{
    class AudioClip
    {
    public:
        //explicit AudioClip(const std::string& path);
        ~AudioClip() = default;

        virtual void Play() = 0;
        virtual void Stop() = 0;

        virtual void Resume() = 0;
        virtual void Pause() = 0;

        virtual void Mute() = 0;
        virtual void UnMute() = 0;


        virtual bool IsPlaying() = 0;
        virtual bool IsLoaded() = 0;

        virtual bool IsPaused() = 0;
        virtual bool IsMuted() = 0;


        virtual float GetVolume() = 0;
        virtual void SetVolume(float volume) = 0;

        virtual int GetSoundId() = 0;


    };
}

