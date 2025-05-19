#include "HealthComponent.h"

#include "Components/Component.h"


using namespace Game;


HealthComponent::HealthComponent(bae::GameObject& owner, float health = 100.f) :
    bae::Component(owner),
    m_Health{ health },
    m_MaxHealth{ m_Health }
{
    if (m_Health < 0.f)
    {
        throw std::runtime_error("Health is negative");
    }

}

bool HealthComponent::IsDead() const
{
    if (m_bIsInvincible)
        return false;

    if (m_Health <= 0)
        return true;

    return false;
}


float HealthComponent::GetHealth()
{
    return m_Health;
}

void HealthComponent::SetHealth(float health)
{
    m_Health = health;
}


float HealthComponent::GetMaxHealth()
{
    return m_MaxHealth;
}

void HealthComponent::SetMaxHealth(float maxHealth)
{
    m_MaxHealth = maxHealth;
}


