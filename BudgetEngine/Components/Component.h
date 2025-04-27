#pragma once
#include "Core/GameObject.h"


namespace bae
{
    class GameObject;
    class Component
    {
    public:
        virtual ~Component() = default;
        explicit Component(GameObject* owner) : m_pOwner{ owner } {}

        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;

        virtual void Update() {};
        virtual void FixedUpdate() {};
        virtual void LateUpdate() {};
        virtual void Render() {};

    private:
        GameObject* m_pOwner;

    protected:
        GameObject* GetOwner() const { return m_pOwner; };

    };
}


