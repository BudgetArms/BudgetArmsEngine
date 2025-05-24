#pragma once
#include "Commands/GameActorCommand.h"
#include "Commands/Command.h"
#include "Core/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"



namespace bae
{

    class FireCommand final : public GameActorCommand
    {
    public:
        FireCommand(GameObject& actor) : GameActorCommand(actor) {};
        virtual ~FireCommand() = default;

        FireCommand(const FireCommand& other) = delete;
        FireCommand(FireCommand&& other) = delete;
        FireCommand& operator=(const FireCommand& other) = delete;
        FireCommand& operator=(FireCommand&& other) = delete;


        virtual void Execute() override
        {
            std::cout << "Firing\n";
            auto test = GetActor()->GetComponent<TransformComponent>();
            test->AddRotation(180);
        }
    };


    class PrintCommand final : public Command
    {
    public:
        PrintCommand(const std::string& text = "PRINTING") :
            Command(),
            m_Text{ text }
        {
        };
        virtual ~PrintCommand() = default;

        PrintCommand(const PrintCommand& other) = delete;
        PrintCommand(PrintCommand&& other) = delete;
        PrintCommand& operator=(const PrintCommand& other) = delete;
        PrintCommand& operator=(PrintCommand&& other) = delete;


        virtual void Execute() override
        {
            std::cout << m_Text << '\n';
        }

    private:
        std::string m_Text;

    };

    class SpriteTesterCommand final : public GameActorCommand
    {
    public:
        SpriteTesterCommand(GameObject& actor) : GameActorCommand(actor) {};
        virtual ~SpriteTesterCommand() = default;

        SpriteTesterCommand(const SpriteTesterCommand& other) = delete;
        SpriteTesterCommand(SpriteTesterCommand&& other) = delete;
        SpriteTesterCommand& operator=(const SpriteTesterCommand& other) = delete;
        SpriteTesterCommand& operator=(SpriteTesterCommand&& other) = delete;


        virtual void Execute() override
        {
            auto* spriteComponent = GetActor()->GetComponent<SpriteComponent>();

            if (spriteComponent)
                //spriteComponent->NextSprite();
                spriteComponent->PreviousSprite();

        }
    };


}

