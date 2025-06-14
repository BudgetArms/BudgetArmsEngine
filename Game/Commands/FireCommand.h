#pragma once
#include "Commands/GameActorCommand.h"
#include "Commands/Command.h"
#include "Core/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"



namespace Game
{

    class PrintCommand final : public bae::Command
    {
    public:
        PrintCommand(const std::string& text = "PRINTING") :
            bae::Command(),
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


    class RotateCommand final : public bae::GameActorCommand
    {
    public:
        RotateCommand(bae::GameObject& actor) :
            bae::GameActorCommand(actor)
        {
        };
        virtual ~RotateCommand() = default;

        RotateCommand(const RotateCommand& other) = delete;
        RotateCommand(RotateCommand&& other) = delete;
        RotateCommand& operator=(const RotateCommand& other) = delete;
        RotateCommand& operator=(RotateCommand&& other) = delete;


        virtual void Execute() override
        {
            std::cout << "Rotating" << '\n';
            auto test = GetActor()->GetComponent<bae::TransformComponent>();
            test->AddRotation(180);
        }
    };




}

