#pragma once

#include "Core/GameObject.hpp"


namespace bae
{
    class GameObject;

    class Component
    {
    public:
        explicit Component(GameObject& owner) : m_Owner{ &owner }
        {
        }

        virtual ~Component() = default;

        Component(const Component&)            = delete;
        Component(Component&&)                 = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&)      = delete;


        virtual void Update() {}
        virtual void FixedUpdate() {}
        virtual void LateUpdate() {}
        virtual void Render() const {}
        virtual void RenderGUI() {}

        [[nodiscard]] virtual GameObject* GetOwner() { return m_Owner; }

    protected:
        GameObject* m_Owner;
    };
}

