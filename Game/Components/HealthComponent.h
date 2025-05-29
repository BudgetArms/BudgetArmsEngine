#pragma once

#include "Components/Component.h"
#include "Core/Subject.h"

class bae::GameObject;

namespace Game
{

    class HealthComponent : public bae::Component, public bae::Subject
    {
    public:
        explicit HealthComponent(bae::GameObject& owner, float maxHealth);
        ~HealthComponent() = default;

        bool IsDead() const;


        float GetHealth();
        void SetHealth(float health);

        float GetMaxHealth();
        void SetMaxHealth(float health);

        void Damage(float damage);


    private:
        float m_Health;
        float m_MaxHealth;

        bool m_bIsInvincible{ false };


    };

}

