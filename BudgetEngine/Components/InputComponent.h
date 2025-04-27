#pragma once
#include <iostream>
#include "Component.h"
#include "Core/GameObject.h"


namespace bae
{
    class InputComponent : public bae::Component
    {
    public:
        InputComponent() :
            bae::Component(nullptr)
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