#include "CoilyStates.h"

#include "Components/SpriteComponent.h"
#include "Singletons/GameTime.h"

#include "Entities/CoilyComponent.h"


using namespace Game::States;



void CoilyStateComponent::Update()
{

}

void CoilyStateComponent::HandleInput(const glm::vec2&)
{
    //if (auto state = )

}



#pragma region Idling


void CoilyIdlingState::Enter()
{

}

void CoilyIdlingState::Exit()
{

}

void CoilyIdlingState::Update()
{
    auto owner = m_CoilyComponent.GetOwner();
    if (!owner)
    {
        std::cout << "IdleState this should not happen\n";
        return;
    }

    auto spriteComp = owner->GetComponent<bae::SpriteComponent>();
    if (!spriteComp)
        return;

    m_ElapsedSec += bae::GameTime::GetInstance().GetDeltaTime();

    if (m_ElapsedSec >= m_SpriteSwitchDelay)
    {
        spriteComp->NextSprite();
        m_ElapsedSec = 0.f;
    }


}

CoilyState* CoilyIdlingState::HandleInput()
{


    return nullptr;
}

#pragma endregion
//*

#pragma region Jumping


void CoilyJumpingState::Enter()
{

}

void CoilyJumpingState::Exit()
{

}

void CoilyJumpingState::Update()
{

}

CoilyState* CoilyJumpingState::HandleInput()
{


    return nullptr;
}


#pragma endregion



#pragma region Falling


void CoilyFallingState::Enter()
{

}

void CoilyFallingState::Exit()
{

}

void CoilyFallingState::Update()
{

}

CoilyState* CoilyFallingState::HandleInput()
{


    return nullptr;
}


#pragma endregion


#pragma region Floating

void CoilyFloatingState::Enter()
{

}

void CoilyFloatingState::Exit()
{

}

void CoilyFloatingState::Update()
{

}

CoilyState* CoilyFloatingState::HandleInput()
{


    return nullptr;
}



#pragma endregion


#pragma region Dying


void CoilyDyingState::Enter()
{

}

void CoilyDyingState::Exit()
{

}

void CoilyDyingState::Update()
{

}

CoilyState* CoilyDyingState::HandleInput()
{


    return nullptr;
}


#pragma endregion



