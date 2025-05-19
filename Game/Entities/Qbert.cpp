#include "Qbert.h"

#include "Core/GameObject.h"


#include "Components/Component.h"
#include "Components/HealthComponent.h"


using namespace Game;

Qbert::Qbert(const std::string& name) :
    bae::GameObject(name)
{
    AddComponent<Game::HealthComponent>(*this, 100.f);
};




