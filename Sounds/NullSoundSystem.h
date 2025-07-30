#pragma once

#include "Sounds/SoundSystem.h"


namespace bae
{
	class SoundSystem;
	class NullSoundSystem : public SoundSystem
	{
	public:
		virtual ~NullSoundSystem() override {};

		virtual SoundID LoadSound(const std::string&) override { return SoundID(); };

		virtual ActiveSoundID Play(SoundID, float) override { return ActiveSoundID(); };
		virtual void Stop(ActiveSoundID) override {};

		virtual void Resume(ActiveSoundID) override {};
		virtual void Pause(ActiveSoundID) override {};

		virtual void Mute(ActiveSoundID) override {};
		virtual void UnMute(ActiveSoundID) override {};

		virtual bool IsLoaded(SoundID) override { return 0; };
		virtual bool IsPlaying(ActiveSoundID) override { return 0; };

		virtual bool IsPaused(ActiveSoundID) override { return 0; };
		virtual bool IsMuted(ActiveSoundID) override { return 0; };


		virtual float GetVolume(ActiveSoundID) override { return 0.f; };
		virtual void SetVolume(ActiveSoundID, float) override {};


		virtual void ResumeAllSounds() override {};
		virtual void PauseAllSounds() override {};
		virtual void StopAllSounds() override {};
		virtual void MuteAllSounds() override {};
		virtual void UnMuteAllSounds() override {};

		virtual void SetVolumeAllSounds(float) override {};

		virtual AudioChunk* GetAudioChunk(SoundID) { return nullptr; };


	};
}


