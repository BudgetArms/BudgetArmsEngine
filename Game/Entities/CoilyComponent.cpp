#include "CoilyComponent.h"

#include <memory>

#include <SDL.h>


#include "Core/GameObject.h"
#include "Core/Subject.h"

#include "Components/Component.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"

#include "Commands/FireCommand.h"
#include "Commands/MoveCommand.h"

#include "Managers/InputManager.h"
#include "Wrappers/Keyboard.h"

#include "Base/CoilyStates.h"


using namespace Game;


CoilyComponent::CoilyComponent(bae::GameObject& owner) :
    bae::Component(owner),
    m_StateComponent{}
{
    m_Owner->AddComponent<Game::HealthComponent>(*m_Owner, 100.f);
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/60496.png", SDL_Rect(0, 32, 128, 32), 8, 8);
    m_Owner->AddComponent<Game::States::CoilyStateComponent>(m_Owner);

    auto* spriteComp = m_Owner->GetComponent<bae::SpriteComponent>();
    if (!spriteComp)
        return;

    auto& location = m_Owner->GetWorldLocation();
    spriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x), static_cast<int>(location.y), 50, 50));
    spriteComp->m_Index = int(CoilySprites::UpLeftCrouched);


    m_StateComponent = m_Owner->GetComponent<Game::States::CoilyStateComponent>();
    if (!m_StateComponent)
        return;


};


void CoilyComponent::Update()
{
    m_StateComponent->Update();
}

void CoilyComponent::Render() const
{

}


void CoilyComponent::HandleInput(const glm::vec2&)
{


}




