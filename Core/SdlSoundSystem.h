#pragma once

#include <memory>
#include <iostream>
#include "Core/SoundSystem.h"


namespace bae
{
	class SdlSoundSystem : public SoundSystem
	{
	public:
		SdlSoundSystem();
		virtual ~SdlSoundSystem();

		virtual SoundID LoadSound(const std::string& path) override;

		virtual ActiveSoundID Play(SoundID soundId, float volume) override;
		virtual void Stop(ActiveSoundID activeSoundId) override;

		virtual void Resume(ActiveSoundID activeSoundId) override;
		virtual void Pause(ActiveSoundID activeSoundId) override;

		virtual void Mute(ActiveSoundID activeSoundId) override;
		virtual void UnMute(ActiveSoundID activeSoundId) override;

		virtual bool IsLoaded(SoundID soundId) override;
		virtual bool IsPlaying(ActiveSoundID activeSoundId) override;

		virtual bool IsPaused(ActiveSoundID activeSoundId) override;
		virtual bool IsMuted(ActiveSoundID activeSoundId) override;

		virtual float GetVolume(ActiveSoundID activeSoundId) override;
		virtual void SetVolume(ActiveSoundID activeSoundId, float volume) override;


		virtual void ResumeAllSounds() override;
		virtual void PauseAllSounds() override;
		virtual void StopAllSounds() override;
		virtual void MuteAllSounds() override;
		virtual void UnMuteAllSounds() override;

		virtual void SetVolumeAllSounds(float volume) override;

		virtual AudioChunk* GetAudioChunk(SoundID soundId) override;


	private:
		// Pimple implmentation
		class Impl;
		std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };


	};
}


