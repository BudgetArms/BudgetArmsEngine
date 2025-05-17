#pragma once
#include "Core/GameObject.h"


namespace bae
{
    class GameObject;
    class Component
    {
    public:
        explicit Component(GameObject& owner) : m_Owner{ &owner } {}
        virtual ~Component() = default;

        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;


        virtual void Update() {};
        virtual void FixedUpdate() {};
        virtual void LateUpdate() {};

        virtual void Render() const {};
        virtual void RenderGUI() {};


    private:


    protected:
        GameObject* m_Owner;


    };
}


