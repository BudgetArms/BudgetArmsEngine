#include "QbertStates.h"

#include "Components/SpriteComponent.h"
#include "Singletons/GameTime.h"

#include "Entities/QbertComponent.h"


using namespace Game::States;


#pragma region Idling


void IdlingState::Enter()
{

}

void IdlingState::Exit()
{

}

void IdlingState::Update()
{
    auto owner = m_QbertComponent.GetOwner();
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

QbertState* IdlingState::HandleInput()
{


    return nullptr;
}

#pragma endregion
//*

#pragma region Jumping


void JumpingState::Enter()
{

}

void JumpingState::Exit()
{

}

void JumpingState::Update()
{

}

QbertState* JumpingState::HandleInput()
{


    return nullptr;
}


#pragma endregion



#pragma region Falling


void FallingState::Enter()
{

}

void FallingState::Exit()
{

}

void FallingState::Update()
{

}

QbertState* FallingState::HandleInput()
{


    return nullptr;
}


#pragma endregion


#pragma region Floating

void FloatingState::Enter()
{

}

void FloatingState::Exit()
{

}

void FloatingState::Update()
{

}

QbertState* FloatingState::HandleInput()
{


    return nullptr;
}



#pragma endregion


#pragma region Dying


void DyingState::Enter()
{

}

void DyingState::Exit()
{

}

void DyingState::Update()
{

}

QbertState* DyingState::HandleInput()
{


    return nullptr;
}


#pragma endregion



//*/
