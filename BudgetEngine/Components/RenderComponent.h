#pragma once
#include <iostream>
#include "Component.h"
#include "Core/GameObject.h"


namespace bae
{
    class RenderComponent : public bae::Component
    {
    public:
        RenderComponent() :
            bae::Component(nullptr)
        {
            std::cout << "Created RenderComponent" << '\n';
        }

        virtual void Render() override
        {
            std::cout << "Fixedupate" << '\n';
        }

    };

}
