#include "HealthComponent.h"

#include "Components/Component.h"
#include "Core/GameObject.h"


using namespace Game;


HealthComponent::HealthComponent(bae::GameObject& owner, float health = 100.f) :
    bae::Component(owner),
    bae::Subject(owner),
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

    // could use dirty flag, but too lazy, and no expensivememory changes anyway
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
    NotifyObservers(bae::EventType::PLAYER_HEALTH_CHANGE);
}


float HealthComponent::GetMaxHealth()
{
    return m_MaxHealth;
}

void HealthComponent::SetMaxHealth(float maxHealth)
{
    m_MaxHealth = maxHealth;
}

void HealthComponent::Damage(float damage)
{
    if (m_bIsInvincible)
        return;

    m_Health -= damage;

    if (m_Health <= 0)
    {
        NotifyObservers(bae::EventType::PLAYER_DIED);
        return;
    }
    NotifyObservers(bae::EventType::PLAYER_HEALTH_CHANGE);
}


