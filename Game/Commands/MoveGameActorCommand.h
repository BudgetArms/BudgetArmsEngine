#pragma once

#include <glm.hpp>

#include "Commands/GameActorCommand.h"

#include "Base/QbertStates.h"


namespace Game
{
    class MoveGameActorCommand : public bae::GameActorCommand
    {
    public:
        MoveGameActorCommand(bae::GameObject& owner, const glm::vec2& direction) :
            bae::GameActorCommand(owner),
            m_Direction{ direction }
        {
            if (glm::length(direction) != 0)
                m_Direction /= glm::length(direction);
        };

        virtual ~MoveGameActorCommand() = default;


        virtual void Execute() override
        {
            auto qbertStateComp = GetActor()->GetComponent<Game::States::QbertStateComponent>();
            if (qbertStateComp)
                qbertStateComp->HandleInput(m_Direction);
        }


    private:
        glm::vec2 m_Direction;

    };

}