#pragma once
#include "Core/GameObject.h"

#include "Components/TransformComponent.h"


namespace bae
{
    class Command
    {
    public:
        explicit Command(GameObject& actor) : m_Actor{ &actor } {}
        virtual ~Command() = default;

        Command(const Command& other) = delete;
        Command(Command&& other) = delete;
        Command& operator=(const Command& other) = delete;
        Command& operator=(Command&& other) = delete;

        virtual void Execute() = 0;


    private:
        GameObject* m_Actor;


    protected:
        GameObject* GetActor() const { return m_Actor; };


    };


    class FireCommand final : public Command
    {
    public:
        FireCommand(GameObject& actor) : Command(actor) {};
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

}



