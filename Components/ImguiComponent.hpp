#pragma once

#include "Components/Component.hpp"


namespace bae
{
    class GameObject;

    class ImguiComponent : public Component
    {
    public:
        explicit ImguiComponent(GameObject& owner) :
            Component(owner)
        {
        }

        ~ImguiComponent() override = default;

        ImguiComponent(const ImguiComponent& other)            = delete;
        ImguiComponent(ImguiComponent&& other)                 = delete;
        ImguiComponent& operator=(const ImguiComponent& other) = delete;
        ImguiComponent& operator=(ImguiComponent&& other)      = delete;

        void RenderGUI() override = 0;
    };
}

