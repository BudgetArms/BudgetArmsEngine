#pragma once

#include "Sounds/AudioClip.h"


namespace bae
{
	class NullAudioClip : public AudioClip
	{
	public:
		NullAudioClip(ActiveSoundID activeSoundId = ActiveSoundID(-1), SoundID soundId = SoundID(-1)) :
			AudioClip(activeSoundId, soundId)
		{
		};

		bool Play() override { return true; };
		void Stop() override {};

		void Resume() override {};
		void Pause() override {};

		void Mute() override {};
		void UnMute() override {};

		bool IsPlaying() const override { return false; };

	    bool IsPaused() const override { return false; };
		bool IsMuted() const override { return false; };

		float GetVolume() const override { return 0.f; };
		void SetVolume(float) override {};

		SoundID GetSoundId() override { return SoundID(-1); };
		ActiveSoundID GetActiveSoundId() override { return ActiveSoundID(-1); };
		int GetChannel() override { return -1; };


	};
}


