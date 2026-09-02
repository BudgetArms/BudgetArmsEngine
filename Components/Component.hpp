#pragma once

#include "Core/GameObject.hpp"


namespace bae
{
    class GameObject;

    class Component
    {
    public:
        explicit Component(GameObject& owner);
        virtual ~Component();

        Component(const Component&)            = delete;
        Component(Component&&)                 = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&)      = delete;


        virtual void Update() {}
        virtual void FixedUpdate() {}
        virtual void LateUpdate() {}
        virtual void Render() const {}
        virtual void RenderGUI() {}

        void Destroy();


        [[nodiscard]] static bool IsValid(const Component* component);
        [[nodiscard]] virtual GameObject* GetOwner();
        [[nodiscard]] bool IsMarkedDeletion() const;

    private:
        bool m_bIsMarkedForDeletion{};

    protected:
        GameObject* m_Owner;
    };
}
