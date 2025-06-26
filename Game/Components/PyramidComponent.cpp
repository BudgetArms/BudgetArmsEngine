#include "PyramidComponent.h"

#include <iostream>

#include <SDL.h>

#include "Core/GameObject.h"
#include "Components/SpriteComponent.h"


using namespace Game;


PyramidComponent::PyramidComponent(bae::GameObject& owner) :
    bae::Component(owner),
    m_SpriteComp{},
    m_Texture{},
    m_Cells{},
    m_Level{}
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/60496.png",
        SDL_Rect(0, 160, 720, 192), 9, 54, glm::ivec2{ 48, 0 });


}

void PyramidComponent::Update()
{

}

void PyramidComponent::Render() const
{

}


