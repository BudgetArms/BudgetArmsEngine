#include "QbertComponent.h"

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

#include "Base/QbertStates.h"


using namespace Game;


QbertComponent::QbertComponent(bae::GameObject& owner) :
    bae::Component(owner)
    , m_State{ std::make_unique<States::IdlingState>(this) }
{
    m_Owner->AddComponent<Game::HealthComponent>(*m_Owner, 100.f);
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/60496.png", SDL_Rect(0, 0, 128, 16), 8, 8);
    auto* spriteComp = m_Owner->GetComponent<bae::SpriteComponent>();
    if (!spriteComp)
        return;

    // I almost forgot I made a 'perfect' spriteComponent system, damn, I am a genius
    // now I need to link that to the states stuff
    auto& location = m_Owner->GetWorldLocation();
    spriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x), static_cast<int>(location.y), 50, 50));
    spriteComp->SetSpriteIndex(int(QBertSprites::UpLeftCrouched));

};


void QbertComponent::Update()
{
    m_State->Update();
}

void QbertComponent::Render() const
{

}




