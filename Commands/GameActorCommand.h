#pragma once
#include "Commands/Command.h"
#include "Core/GameObject.h"


namespace bae
{
    class GameActorCommand : public Command
    {
    public:
        explicit GameActorCommand(GameObject& actor) :
            Command(),
            m_Actor{ &actor }
        {
        }

        ~GameActorCommand() override = default;

        GameActorCommand(const GameActorCommand& other)            = delete;
        GameActorCommand(GameActorCommand&& other)                 = delete;
        GameActorCommand& operator=(const GameActorCommand& other) = delete;
        GameActorCommand& operator=(GameActorCommand&& other)      = delete;


        void Execute() override = 0;

    private:
        GameObject* m_Actor;

    protected:
        [[nodiscard]] GameObject* GetActor() const { return m_Actor; }
    };
}


