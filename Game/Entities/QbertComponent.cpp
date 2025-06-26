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

#include "Core/Renderer.h"
#include "Wrappers/Texture2D.h"


using namespace Game;


QbertComponent::QbertComponent(bae::GameObject& owner) :
    bae::Component(owner),
    m_StateComponent{}
{
    m_Owner->AddComponent<Game::HealthComponent>(*m_Owner, 100.f);
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/60496.png",
        SDL_Rect(0, 0, 128, 16), 8, 8);

    m_Owner->AddComponent<Game::States::QbertStateComponent>(m_Owner);

    auto* spriteComp = m_Owner->GetComponent<bae::SpriteComponent>();
    if (!spriteComp)
        return;

    auto& location = m_Owner->GetWorldLocation();
    spriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x), static_cast<int>(location.y), 50, 50));
    spriteComp->m_Index = int(QbertSprites::UpLeftCrouched);


    m_StateComponent = m_Owner->GetComponent<Game::States::QbertStateComponent>();
    if (!m_StateComponent)
        return;


};


void QbertComponent::Update()
{
    m_StateComponent->Update();
}

void QbertComponent::Render() const
{
    auto* spriteComp = m_Owner->GetComponent<bae::SpriteComponent>();
    if (!spriteComp)
        return;

    //auto& location = m_Owner->GetWorldLocation();
    //spriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x + 50.f), static_cast<int>(location.y + 50.f), 50, 50));
    //spriteComp->m_Index = int(QbertSprites::DownLeftCrouched);
    //spriteComp->Render();

    //spriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x + 200.f), static_cast<int>(location.y), 50, 50));
    //spriteComp->m_Index = int(QbertSprites::DownRightCrouched);
    //spriteComp->Render();



}


void QbertComponent::HandleInput(const glm::vec2&)
{


}




