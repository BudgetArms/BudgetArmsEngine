#pragma once

#include <memory>
#include <iostream>
#include "Core/AudioClip.h"
#include "Core/SoundSystem.h"
#include "Core/HelperFunctions.h"


namespace bae
{
	template<typename RealAudioClipType,
		typename = std::enable_if_t<std::is_base_of_v<bae::AudioClip, RealAudioClipType> &&
		!std::is_same_v<bae::AudioClip, RealAudioClipType>>>
		class LoggingAudioClip : public AudioClip
	{
	public:

		LoggingAudioClip(bae::ActiveSoundID activeSoundId, bae::SoundID soundId) :
			AudioClip(),
			m_RealAudioClip{ }
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip = std::make_unique<RealAudioClipType>(activeSoundId, soundId);
		};

		virtual ~LoggingAudioClip()
		{
			std::cout << GetFunctionName() << '\n';
		};

		virtual bool Play() override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->Play();
		};
		virtual void Stop() override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->Stop();
		};

		virtual void Resume() override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->Resume();
		};
		virtual void Pause() override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->Pause();
		};

		virtual void Mute() override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->Mute();
		};
		virtual void UnMute() override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->UnMute();
		};


		virtual bool IsPlaying() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->IsPlaying();
		};

		virtual bool IsPaused() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->IsPaused();
		};
		virtual bool IsMuted() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->IsMuted();
		};


		virtual float GetVolume() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->GetVolume();
		};

		virtual void SetVolume(float volume) override
		{
			std::cout << GetFunctionName() << '\n';
			m_RealAudioClip->SetVolume(volume);
		};


		virtual SoundID GetSoundId() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->GetSoundId();
		};

		virtual ActiveSoundID GetActiveSoundId() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->GetActiveSoundId();
		};

		virtual int GetChannel() override
		{
			std::cout << GetFunctionName() << '\n';
			return m_RealAudioClip->GetChannel();
		};


	private:
		std::unique_ptr<AudioClip> m_RealAudioClip;


	};

}

