#pragma once

#include <string>
#include "Core/SoundSystem.h"


namespace bae
{
	class AudioClip
	{
	public:
		//explicit AudioClip(const std::string& path);
		virtual ~AudioClip() = default;

		// return indicates sucess creation
		virtual bool Play() = 0;
		virtual void Stop() = 0;

		virtual void Resume() = 0;
		virtual void Pause() = 0;

		virtual void Mute() = 0;
		virtual void UnMute() = 0;


		virtual bool IsPlaying() const = 0;

		virtual bool IsPaused() const = 0;
		virtual bool IsMuted() const = 0;


		virtual float GetVolume() const = 0;
		virtual void SetVolume(float volume) = 0;

		virtual SoundID GetSoundId() = 0;
		virtual ActiveSoundID GetActiveSoundId() = 0;
		virtual int GetChannel() = 0;


	};
}

