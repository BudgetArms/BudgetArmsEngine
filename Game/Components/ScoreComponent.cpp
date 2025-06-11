#include "ScoreComponent.h"

#include "Core/Subject.h"
#include "Core/EventTypes.h"


using namespace Game;

ScoreComponent::ScoreComponent(bae::GameObject& owner) :
    Component(owner),
    Subject(owner)
{
}


void ScoreComponent::AddScore(int score)
{
    m_Score += score;
    NotifyObservers(bae::EventType::PLAYER_SCORE_CHANGE);
};

void ScoreComponent::SubtractScore(int score)
{
    m_Score -= score;
    NotifyObservers(bae::EventType::PLAYER_SCORE_CHANGE);
};
