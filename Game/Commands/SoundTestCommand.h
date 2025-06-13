#pragma once

#include <iostream>

#include "Commands/Command.h"
#include "Core/ServiceLocator.h"

#include "Base/SoundEvents.h"


namespace Game
{

    class SoundTestCommand : public bae::Command
    {
    public:
        SoundTestCommand(Game::Sounds::SoundEvents sound) :
            bae::Command(),
            m_SoundId{ Game::Sounds::GetSoundId(sound) }
        {
        };

        virtual ~SoundTestCommand() = default;

        SoundTestCommand(const SoundTestCommand& other) = delete;
        SoundTestCommand(SoundTestCommand&& other) = delete;
        SoundTestCommand& operator=(const SoundTestCommand& other) = delete;
        SoundTestCommand& operator=(SoundTestCommand&& other) = delete;

        virtual void Execute() override = 0;


    protected:
        int m_SoundId;


    };


    class SoundPlayCommand final : public Game::SoundTestCommand
    {
    public:
        SoundPlayCommand(Game::Sounds::SoundEvents sound) :
            Game::SoundTestCommand(sound)
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.Play(m_SoundId, 0.5f);
        }
    };

    class SoundStopCommand final : public Game::SoundTestCommand
    {
    public:
        SoundStopCommand(Game::Sounds::SoundEvents sound) :
            Game::SoundTestCommand(sound)
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.Resume(m_SoundId);
        }
    };

    class SoundResumeCommand final : public Game::SoundTestCommand
    {
    public:
        SoundResumeCommand(Game::Sounds::SoundEvents sound) :
            Game::SoundTestCommand(sound)
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.Resume(m_SoundId);
        }
    };

    class SoundPauseCommand final : public Game::SoundTestCommand
    {
    public:
        SoundPauseCommand(Game::Sounds::SoundEvents sound) :
            Game::SoundTestCommand(sound)
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.Pause(m_SoundId);
        }
    };

    class SoundMuteCommand final : public Game::SoundTestCommand
    {
    public:
        SoundMuteCommand(Game::Sounds::SoundEvents sound) :
            Game::SoundTestCommand(sound)
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.Pause(m_SoundId);
        }
    };

    class SoundUnMuteCommand final : public Game::SoundTestCommand
    {
    public:
        SoundUnMuteCommand(Game::Sounds::SoundEvents sound) :
            Game::SoundTestCommand(sound)
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.UnMute(m_SoundId);
        }
    };


    class SoundStopAllSoundsCommand final : public bae::Command
    {
    public:
        SoundStopAllSoundsCommand() :
            bae::Command()
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.StopAllSounds();
        }
    };

    class SoundMuteAllSoundsCommand final : public bae::Command
    {
    public:
        SoundMuteAllSoundsCommand() :
            bae::Command()
        {
        };

        virtual void Execute() override
        {
            auto& soundSystem = bae::ServiceLocator::GetSoundSystem();
            soundSystem.MuteAllSounds();
        }
    };


}

