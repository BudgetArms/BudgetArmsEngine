#pragma once
#include "Commands/GameActorCommand.h"
#include "Commands/Command.h"
#include "Core/GameObject.h"
#include "Components/TransformComponent.h"



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
        PrintCommand() : Command() {};
        virtual ~PrintCommand() = default;

        PrintCommand(const PrintCommand& other) = delete;
        PrintCommand(PrintCommand&& other) = delete;
        PrintCommand& operator=(const PrintCommand& other) = delete;
        PrintCommand& operator=(PrintCommand&& other) = delete;


        virtual void Execute() override
        {
            std::cout << "PRINTING\n";
        }
    };

}

