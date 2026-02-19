#pragma once

#include <memory>

#include "Sounds/AudioClip.h"


namespace bae
{
	class SdlAudioClip final : public AudioClip
	{
	public:
		SdlAudioClip(ActiveSoundID activeId, SoundID soundId);
        ~SdlAudioClip() override; // empty, needed for Pimpl

		bool Play() override;
	    void Stop() override;

		void Resume() override;
		void Pause() override;

		void Mute() override;
		void UnMute() override;


		bool IsPlaying() const override;

		bool IsPaused() const override;
		bool IsMuted() const override;


		float GetVolume() const override;
		void SetVolume(float volume) override;

		SoundID GetSoundId() override;
		ActiveSoundID GetActiveSoundId() override;
		int GetChannel() override;


	private:
		class Impl;
		std::unique_ptr<Impl> m_Pimpl;


	};
}


