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

        ImguiComponent(const ImguiComponent&)            = delete;
        ImguiComponent(ImguiComponent&&)                 = delete;
        ImguiComponent& operator=(const ImguiComponent&) = delete;
        ImguiComponent& operator=(ImguiComponent&&)      = delete;

        void RenderGUI() override = 0;
    };
}

