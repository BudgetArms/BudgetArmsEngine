#pragma once

#include "Commands/GameActorCommand.h"
#include "Components/SpriteComponent.h"


namespace Game
{

    class SpriteTesterCommand final : public bae::GameActorCommand
    {
    public:
        SpriteTesterCommand(bae::GameObject& actor) :
            bae::GameActorCommand(actor)
        {
        };
        virtual ~SpriteTesterCommand() = default;

        SpriteTesterCommand(const SpriteTesterCommand& other) = delete;
        SpriteTesterCommand(SpriteTesterCommand&& other) = delete;
        SpriteTesterCommand& operator=(const SpriteTesterCommand& other) = delete;
        SpriteTesterCommand& operator=(SpriteTesterCommand&& other) = delete;


        virtual void Execute() override
        {
            auto* spriteComponent = GetActor()->GetComponent<bae::SpriteComponent>();

            if (spriteComponent)
                //spriteComponent->NextSprite();
                spriteComponent->PreviousSprite();

        }
    };
}


