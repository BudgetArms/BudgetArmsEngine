#pragma once

#include "Commands/Command.hpp"
#include "Core/GameObject.hpp"


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

        GameActorCommand(const GameActorCommand&)            = delete;
        GameActorCommand(GameActorCommand&&)                 = delete;
        GameActorCommand& operator=(const GameActorCommand&) = delete;
        GameActorCommand& operator=(GameActorCommand&&)      = delete;


        void Execute() override = 0;

        [[nodiscard]] bool IsValid() const override { return !m_Actor->IsMarkedForDeletion(); }

    private:
        GameObject* m_Actor;

    protected:
        [[nodiscard]] GameObject* GetActor() const { return m_Actor; }
    };
}


