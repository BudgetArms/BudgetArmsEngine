#pragma once

#include <memory>

#include "Sounds/SoundStructs.h"
#include "Sounds/SoundSystem.h"


namespace bae
{
	class LoggingSoundSystem : public SoundSystem
	{
	public:
        explicit LoggingSoundSystem(std::unique_ptr<SoundSystem> soundSystem);
        ~LoggingSoundSystem() override;

		SoundID LoadSound(const std::string& path) override;

		ActiveSoundID Play(SoundID soundId, float volume) override;
		void Stop(ActiveSoundID activeSoundId) override;

		void Resume(ActiveSoundID activeSoundId) override;
		void Pause(ActiveSoundID activeSoundId) override;

		void Mute(ActiveSoundID activeSoundId) override;
		void UnMute(ActiveSoundID activeSoundId) override;

	    bool IsLoaded(SoundID soundId) override;
		bool IsPlaying(ActiveSoundID activeSoundId) override;

		bool IsPaused(ActiveSoundID activeSoundId) override;
	    bool IsMuted(ActiveSoundID activeSoundId) override;

		float GetVolume(ActiveSoundID activeSoundId) override;
		void  SetVolume(ActiveSoundID activeSoundId, float volume) override;

		void ResumeAllSounds() override;
	    void PauseAllSounds() override;
		void StopAllSounds() override;
		void MuteAllSounds() override;
		void UnMuteAllSounds() override;

		void SetVolumeAllSounds(float volume) override;

		AudioChunk* GetAudioChunk(SoundID soundId) override;


	private:
		std::unique_ptr<SoundSystem> m_RealSoundSystem;


	};
}

