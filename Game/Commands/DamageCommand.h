#pragma once

#include "Core/GameObject.h"
#include "Commands/GameActorCommand.h"

#include "Components/HealthComponent.h"


namespace Game
{
    class DamageCommand : public bae::GameActorCommand
    {
    public:
        DamageCommand(bae::GameObject& actor, float damage) :
            bae::GameActorCommand(actor),
            m_Damage{ damage }
        {
        }

        virtual ~DamageCommand() = default;

        DamageCommand(const DamageCommand& other) = delete;
        DamageCommand(DamageCommand&& other) = delete;
        DamageCommand& operator=(const DamageCommand& other) = delete;
        DamageCommand& operator=(DamageCommand&& other) = delete;


        virtual void Execute() override
        {
            auto* healthComponent = GetActor()->GetComponent<HealthComponent>();
            if (!healthComponent)
                return;

            healthComponent->Damage(m_Damage);
        };


    private:
        float m_Damage;


    };

}