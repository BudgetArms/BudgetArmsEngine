#pragma once

#include <string>
#include "Wrappers/AudioChunk.h"

namespace bae
{
	struct SoundID
	{
		int ID;
	};

	struct ActiveSoundID
	{
		int ID{ -1 };

		static ActiveSoundID GenerateActiveSoundID()
		{
			return ActiveSoundID{ .ID = rand() };
		}
	};

	constexpr inline bool operator==(const SoundID& lhs, const SoundID& rhs)
	{
		return lhs.ID == rhs.ID;
	}
	inline bool operator==(const ActiveSoundID& lhs, const ActiveSoundID& rhs)
	{
		return lhs.ID == rhs.ID;
	}


	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;

		virtual SoundID LoadSound(const std::string& path) = 0;

		virtual ActiveSoundID Play(SoundID soundId, float volume) = 0;
		virtual void Stop(ActiveSoundID activeSoundId) = 0;

		virtual void Resume(ActiveSoundID activeSoundId) = 0;
		virtual void Pause(ActiveSoundID activeSoundId) = 0;

		virtual void Mute(ActiveSoundID activeSoundId) = 0;
		virtual void UnMute(ActiveSoundID activeSoundId) = 0;

		virtual bool IsLoaded(SoundID soundId) = 0;
		virtual bool IsPlaying(ActiveSoundID activeSoundId) = 0;

		virtual bool IsPaused(ActiveSoundID activeSoundId) = 0;
		virtual bool IsMuted(ActiveSoundID activeSoundId) = 0;

		virtual float GetVolume(ActiveSoundID activeSoundId) = 0;
		virtual void SetVolume(ActiveSoundID activeSoundId, float volume) = 0;


		// there is no PlayAllSounds bc that would be insane with 100+ sounds 
		virtual void ResumeAllSounds() = 0;
		virtual void PauseAllSounds() = 0;
		virtual void StopAllSounds() = 0;
		virtual void MuteAllSounds() = 0;
		virtual void UnMuteAllSounds() = 0;

		virtual void SetVolumeAllSounds(float volume) = 0;


		virtual AudioChunk* GetAudioChunk(SoundID soundId) = 0;


	};
}


template <>
struct std::hash<bae::SoundID>
{
	std::size_t operator()(const bae::SoundID& soundId) const noexcept
	{
		return std::hash<int>{}(soundId.ID);
	}
};

template <>
struct std::hash<bae::ActiveSoundID>
{
	std::size_t operator()(const bae::ActiveSoundID& activeSoundId) const noexcept
	{
		return std::hash<int>{}(activeSoundId.ID);
	}
};


