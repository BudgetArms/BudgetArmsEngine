#pragma once

#include <functional>

#include "Components/Component.h"


namespace bae
{
    class GameObject;
    class ImguiComponent : public Component
    {
    public:
        explicit ImguiComponent(GameObject& owner);
        virtual ~ImguiComponent() = default;

        ImguiComponent(const ImguiComponent& other) = delete;
        ImguiComponent(ImguiComponent&& other) = delete;
        ImguiComponent& operator=(const ImguiComponent& other) = delete;
        ImguiComponent& operator=(ImguiComponent&& other) = delete;


        virtual void RenderGUI() = 0;


    private:


    protected:


    };

}

