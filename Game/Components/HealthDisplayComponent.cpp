#include "HealthDisplayComponent.h"

#include "Core/Subject.h"
#include "Core/GameObject.h"
#include "Components/HealthComponent.h"


using namespace Game;

HealthDisplayComponent::HealthDisplayComponent(bae::GameObject& owner, std::shared_ptr<bae::Font> font) :
    bae::TextComponent(owner, "Health: xxx", font)
{
}

void HealthDisplayComponent::Notify(bae::Event event, bae::Subject* subject)
{
    if (event != bae::Event::PLAYER_DIED && event != bae::Event::PLAYER_HEALTH_CHANGE)
        return;

    if (event == bae::Event::PLAYER_DIED)
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


