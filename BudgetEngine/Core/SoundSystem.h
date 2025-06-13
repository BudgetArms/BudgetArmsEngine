#pragma once

#include <string>


namespace bae
{
    class SoundSystem
    {
    public:
        SoundSystem() = default;
        virtual ~SoundSystem() = default;

        virtual int LoadSound(const std::string& path) = 0;

        virtual void Play(int soundId, float volume) = 0;
        virtual void Stop(int soundId) = 0;
        virtual void Mute(int soundId) = 0;

        virtual bool IsLoaded(int soundId) = 0;
        virtual bool IsPlaying(int soundId) = 0;


        virtual void StopAllSounds() = 0;
        virtual void MuteAllSounds() = 0;


    };
}


