#pragma once

#include <memory>

#include <glm.hpp>

#include "Core/State.h"
#include "Components/Component.h"


namespace Game
{
    class QbertComponent;
}

namespace Game::States
{
    // yes, this goes about 1 spot difference
    class QbertState;

    // Credit to Adriaan 
    class QbertStateComponent final : public bae::Component
    {
    public:
        QbertStateComponent(bae::GameObject* owner) :
            bae::Component(*owner)
        {
        }
        virtual ~QbertStateComponent() = default;

        virtual void Update() override;
        virtual void HandleInput(const glm::vec2& dir);


    private:
        std::unique_ptr<QbertState> m_State{};

    };


    // partially cursed structure, bc Coily also has an Idling, Jumping, etc State
    class QbertState : public bae::State
    {
    public:
        QbertState(QbertComponent* qbertComponent) :
            m_QbertComponent{ *qbertComponent }
        {
        }
        virtual ~QbertState() = default;

        virtual void Update() = 0;
        virtual QbertState* HandleInput() = 0;

    protected:
        QbertComponent& m_QbertComponent;
    };


    class QbertIdlingState final : public QbertState
    {
    public:
        explicit QbertIdlingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~QbertIdlingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:
        float m_ElapsedSec{};
        const float m_SpriteSwitchDelay{ 2.f };

    };

    class QbertJumpingState final : public QbertState
    {
    public:
        explicit QbertJumpingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~QbertJumpingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:


    };

    class QbertFallingState final : public QbertState
    {
    public:
        explicit QbertFallingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~QbertFallingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:

    };

    class QbertFloatingState final : public QbertState
    {
    public:
        explicit QbertFloatingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~QbertFloatingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:

    };

    class QbertDyingState final : public QbertState
    {
    public:
        explicit QbertDyingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~QbertDyingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;



    private:

    };

}


