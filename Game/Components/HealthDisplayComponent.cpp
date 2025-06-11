#include "HealthDisplayComponent.h"

#include "Core/Subject.h"
#include "Core/GameObject.h"
#include "Components/HealthComponent.h"


using namespace Game;

HealthDisplayComponent::HealthDisplayComponent(bae::GameObject& owner, std::shared_ptr<bae::Font> font) :
    bae::TextComponent(owner, "Health: xxx", font)
{
}

void HealthDisplayComponent::Notify(bae::EventType event, bae::Subject* subject)
{
    if (event != bae::EventType::PLAYER_DIED && event != bae::EventType::PLAYER_HEALTH_CHANGE)
        return;

    if (event == bae::EventType::PLAYER_DIED)
    {
        SetText("Player died");
        return;
    }


    auto gameObj = subject->GetGameObject();

    auto* scoreComponent = gameObj->GetComponent<HealthComponent>();
    if (!scoreComponent)
        return;

    SetText("Health: " + std::to_string(scoreComponent->GetHealth()));


}


