#include "ScoreDisplayComponent.h"

#include "Core/Subject.h"
#include "Components/ScoreComponent.h"


using namespace Game;

ScoreDisplayComponent::ScoreDisplayComponent(bae::GameObject& owner, std::shared_ptr<bae::Font> font) :
    bae::TextComponent(owner, "Score: xxx", font)
{
}

void ScoreDisplayComponent::Notify(bae::EventType event, bae::Subject* subject)
{
    if (event != bae::EventType::PLAYER_SCORE_CHANGE)
        return;

    auto* scoreComponent = subject->GetGameObject()->GetComponent<ScoreComponent>();
    if (!scoreComponent)
        return;

    SetText("Score: " + std::to_string(scoreComponent->GetScore()));

}



