#pragma once
#include "Core/State.h"


namespace Game
{
    class QbertComponent;
}

namespace Game::States
{
    // partially cursed structure, bc Coily also has an Idling, Jumping, etc State

    class QbertState : bae::State
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


    class IdlingState final : public QbertState
    {
    public:
        explicit IdlingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~IdlingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:
        float m_ElapsedSec{};
        const float m_SpriteSwitchDelay{ 3.f };

    };

    class JumpingState final : public QbertState
    {
    public:
        explicit JumpingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~JumpingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:


    };

    class FallingState final : public QbertState
    {
    public:
        explicit FallingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~FallingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:

    };

    class FloatingState final : public QbertState
    {
    public:
        explicit FloatingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~FloatingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;


    private:

    };

    class DyingState final : public QbertState
    {
    public:
        explicit DyingState(QbertComponent* qbertComponent) :
            QbertState(qbertComponent)
        {
        };
        virtual ~DyingState() = default;

        virtual void Enter() override;
        virtual void Exit() override;

        virtual void Update() override;
        virtual QbertState* HandleInput() override;



    private:

    };

}


