#pragma once

#include <string>
#include <memory>
#include "Core/AudioClip.h"


namespace bae
{
	// Note: I want to have a logging system, just like SoundSystem has one, but for AudioClips
	class SdlAudioClip : public AudioClip
	{
	public:
		SdlAudioClip(ActiveSoundID activeId, SoundID soudId);
		~SdlAudioClip(); // empty, needed for Pimpl

		virtual bool Play() override;
		virtual void Stop() override;

		virtual void Resume() override;
		virtual void Pause() override;

		virtual void Mute() override;
		virtual void UnMute() override;


		virtual bool IsPlaying() const override;

		virtual bool IsPaused() const override;
		virtual bool IsMuted() const override;


		virtual float GetVolume() const override;
		virtual void SetVolume(float volume) override;

		virtual SoundID GetSoundId() override;
		virtual ActiveSoundID GetActiveSoundId() override;
		virtual int GetChannel() override;
		// channel probably also should a be a class bc other audio libraries might not 
		// use int to identify their channels


	private:
		class Impl;
		std::unique_ptr<Impl> m_Pimpl;


	};
}


