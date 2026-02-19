#pragma once

#include "Sounds/SoundSystem.h"


namespace bae
{
	class SoundSystem;
	class NullSoundSystem : public SoundSystem
	{
	public:
		~NullSoundSystem() override {};

		SoundID LoadSound(const std::string&) override { return SoundID(); };

		ActiveSoundID Play(SoundID, float) override { return ActiveSoundID(); };
		void Stop(ActiveSoundID) override {};

		void Resume(ActiveSoundID) override {};
		void Pause(ActiveSoundID) override {};

		void Mute(ActiveSoundID) override {};
		void UnMute(ActiveSoundID) override {};

		bool IsLoaded(SoundID) override { return false; };
		bool IsPlaying(ActiveSoundID) override { return false; };

		bool IsPaused(ActiveSoundID) override { return false; };
		bool IsMuted(ActiveSoundID) override { return false; };


		float GetVolume(ActiveSoundID) override { return 0.f; };
		void SetVolume(ActiveSoundID, float) override {};


	    void ResumeAllSounds() override {};
		void PauseAllSounds() override {};
	    void StopAllSounds() override {};
		void MuteAllSounds() override {};
		void UnMuteAllSounds() override {};

		void SetVolumeAllSounds(float) override {};

        AudioChunk* GetAudioChunk(SoundID) override { return nullptr; };


	};
}


