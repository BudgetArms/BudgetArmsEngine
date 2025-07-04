﻿#pragma once

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

        virtual void Resume(int soundId) = 0;
        virtual void Pause(int soundId) = 0;

        virtual void Mute(int soundId) = 0;
        virtual void UnMute(int soundId) = 0;

        virtual bool IsLoaded(int soundId) = 0;
        virtual bool IsPlaying(int soundId) = 0;

        virtual bool IsPaused(int soundId) = 0;
        virtual bool IsMuted(int soundId) = 0;


        // there is no PlayAllSounds bc that would be insane with 100+ sounds 
        virtual void ResumeAllSounds() = 0;
        virtual void PauseAllSounds() = 0;
        virtual void StopAllSounds() = 0;
        virtual void MuteAllSounds() = 0;
        virtual void UnMuteAllSounds() = 0;


    };
}


