#pragma once

#include <memory>

#include <glm.hpp>

#include "Core/State.h"
#include "Components/Component.h"


namespace Game
{
    class CoilyComponent;
}

namespace Game::States
{
    // yes, this goes about 1 spot difference
    class CoilyState;

    class CoilyStateComponent final : public bae::Component
    {
    public:
        CoilyStateComponent(bae::GameObject* owner) :
            bae::Component(*owner)
        {
        }
        virtual ~CoilyStateComponent() = default;

        virtual void Update() override;
        virtual void HandleInput(const glm::vec2& dir);


    private:
        std::unique_ptr<CoilyState> m_State{};

    };


    // partially cursed structure, bc Coily also has an Idling, Jumping, etc State
    class CoilyState : public bae::State
    {
    public:
        CoilyState(CoilyComponent* CoilyComponent) :
            m_CoilyComponent{ *CoilyComponent }
        {
        }
        virtual ~CoilyState() = default;

        virtual void Update() = 0;
        virtual CoilyState* HandleInput() = 0;

    protected:
        CoilyComponent& m_CoilyComponent;
    };


    class CoilyIdlingState final : public CoilyState
    {
    public:
        explicit CoilyIdlingState(CoilyComponent* CoilyComponent) :
            CoilyState(CoilyComponent)
        {
        };
        virtual ~CoilyIdlingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual CoilyState* HandleInput() override;


    private:
        float m_ElapsedSec{};
        const float m_SpriteSwitchDelay{ 2.f };

    };

    class CoilyJumpingState final : public CoilyState
    {
    public:
        explicit CoilyJumpingState(CoilyComponent* CoilyComponent) :
            CoilyState(CoilyComponent)
        {
        };
        virtual ~CoilyJumpingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual CoilyState* HandleInput() override;


    private:


    };

    class CoilyFallingState final : public CoilyState
    {
    public:
        explicit CoilyFallingState(CoilyComponent* CoilyComponent) :
            CoilyState(CoilyComponent)
        {
        };
        virtual ~CoilyFallingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual CoilyState* HandleInput() override;


    private:

    };

    class CoilyFloatingState final : public CoilyState
    {
    public:
        explicit CoilyFloatingState(CoilyComponent* CoilyComponent) :
            CoilyState(CoilyComponent)
        {
        };
        virtual ~CoilyFloatingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual CoilyState* HandleInput() override;


    private:

    };

    class CoilyDyingState final : public CoilyState
    {
    public:
        explicit CoilyDyingState(CoilyComponent* CoilyComponent) :
            CoilyState(CoilyComponent)
        {
        };
        virtual ~CoilyDyingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual CoilyState* HandleInput() override;



    private:

    };

}


