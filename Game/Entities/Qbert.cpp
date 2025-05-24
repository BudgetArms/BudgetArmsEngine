#include "Qbert.h"

#include <memory>

#include "Core/GameObject.h"
#include <SDL.h>

#include "Components/Component.h"
#include "Components/HealthComponent.h"
#include "Components/SpriteComponent.h"

#include "Commands/FireCommand.h"
#include "Commands/MoveCommand.h"

#include "Managers/InputManager.h"
#include "Wrappers/Keyboard.h"


using namespace Game;

Qbert::Qbert(const std::string& name) :
    bae::GameObject(name)
{
    AddComponent<Game::HealthComponent>(*this, 100.f);
    AddComponent<bae::SpriteComponent>(*this, "Textures/60496.png", SDL_Rect(0, 0, 128, 16), 8, 8);
    auto& location = GetWorldLocation();
    auto* spriteComp = GetComponent<bae::SpriteComponent>();
    spriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x), static_cast<int>(location.y), 50, 50));


};




