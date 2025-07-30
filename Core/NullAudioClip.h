#pragma once

#include "Core/AudioClip.h"


namespace bae
{
	class NullAudioClip : public AudioClip
	{
		virtual bool Play() override {};
		virtual void Stop() override {};

		virtual void Resume() override {};
		virtual void Pause() override {};

		virtual void Mute() override {};
		virtual void UnMute() override {};

		virtual bool IsPlaying() const override { return false; };

		virtual bool IsPaused() const override { return false; };
		virtual bool IsMuted() const override { return false; };

		virtual float GetVolume() const override { return 0.f; };
		virtual void SetVolume(float) override {};

		virtual SoundID GetSoundId() override { return SoundID(-1); };
		virtual ActiveSoundID GetActiveSoundId() override { return ActiveSoundID(-1); };
		virtual int GetChannel() override { return -1; };


	};
}

