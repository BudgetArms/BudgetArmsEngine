#pragma once
#include <functional>


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

	enum class SoundEventType
	{
		Play,
		Stop,
		Resume,
		Pause,
		Mute,
		UnMute,
		SetVolume,

		StopAll,
		ResumeAll,
		PauseAll,
		MuteAll,
		UnMuteAll,
		SetVolumeAll,
	};

	struct SoundEventData
	{
		SoundEventType Type;
		SoundID SoundID{ -1 };
		ActiveSoundID ActiveSoundID{ -1 };
		float Volume{ 1 };
	};


	constexpr inline bool operator==(const SoundID& lhs, const SoundID& rhs)
	{
		return lhs.ID == rhs.ID;
	}

	inline bool operator==(const ActiveSoundID& lhs, const ActiveSoundID& rhs)
	{
		return lhs.ID == rhs.ID;
	}


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


