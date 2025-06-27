#pragma once
#include <iostream>

#include "Components/Component.h"
#include "Core/GameObject.h"


namespace bae
{
    class InputComponent : public Component
    {
    public:
        explicit InputComponent(GameObject& owner) :
            bae::Component(owner)
        {
            std::cout << "Created InputComponent" << '\n';
        }

        virtual void Update() override
        {
            std::cout << "Update" << '\n';
        }

        virtual void FixedUpdate() override
        {
            std::cout << "Fixedupate" << '\n';
        }
        virtual void Render() override
        {
            std::cout << "Fixedupate" << '\n';
        }

    };

}