#pragma once

#include "Core/GameObject.h"
#include "Commands/GameActorCommand.h"

#include "Components/ScoreComponent.h"


namespace Game
{
    class ScoreCommand : public bae::GameActorCommand
    {
    public:
        ScoreCommand(bae::GameObject& actor, int scoreChange) :
            bae::GameActorCommand(actor),
            m_ScoreChange{ scoreChange }
        {
        }

        virtual ~ScoreCommand() = default;

        ScoreCommand(const ScoreCommand& other) = delete;
        ScoreCommand(ScoreCommand&& other) = delete;
        ScoreCommand& operator=(const ScoreCommand& other) = delete;
        ScoreCommand& operator=(ScoreCommand&& other) = delete;


        virtual void Execute() override
        {
            auto* scoresComponent = GetActor()->GetComponent<ScoreComponent>();
            if (!scoresComponent)
                return;

            scoresComponent->AddScore(m_ScoreChange);
        };


    private:
        int m_ScoreChange{};


    };

}