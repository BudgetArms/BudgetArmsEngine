#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <glm.hpp>

//#include "Components/TransformComponent.h"
//#include "Components/Component.h"



namespace bae
{
    class Texture2D;
    class Component;
    class TransformComponent;

    class GameObject
    {
    public:

        explicit GameObject(const std::string& name);
        virtual ~GameObject();

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;


        virtual void Update();
        virtual void FixedUpdate();
        virtual void LateUpdate();
        virtual void Render() const;


        // Mark for destruction
        void Destroy();

        // this is a way more accurate name, Remove Child sounds too much like destroying
        void AttachChild(GameObject* child, bool freezeLocation = true, bool freezeRotation = true, bool freezeScale = true);
        void DetachChild(GameObject* child, bool updateChildrenOfChildLocations = true);

        bool IsChild(const GameObject* child) const;

        std::string GetName() const { return m_Name; };

        GameObject* GetParent() const;
        void SetParent(GameObject* newParent, bool keepLocation);

        constexpr bool IsMarkedForDeletion() const { return m_MarkedForDeletion; };


        const glm::vec3& GetWorldLocation();
        float GetWorldRotation();
        const glm::vec2& GetWorldScale();

        const glm::vec3& GetLocalLocation() const;
        float GetLocalRotation() const;
        const glm::vec2& GetLocalScale() const;

        void SetWorldLocation(const glm::vec3& location);
        void SetWorldRotation(float rotation);
        void SetWorldScale(const glm::vec2& scale);

        void SetLocalLocation(const glm::vec3& location);
        void SetLocalRotation(float rotation);
        void SetLocalScale(const glm::vec2& scale);

        void AddLocation(const glm::vec3& location);
        void AddRotation(float rotation);
        void AddScale(const glm::vec2& scale);

        constexpr void SetLocationDirty();
        constexpr void SetRotationDirty();
        constexpr void SetScaleDirty();


        template<typename ComponentType, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
            !std::is_same_v<Component, ComponentType>>>
            void AddComponent(Args&&... args)
        {
            // We don't need two components of the same type
            if (GameObject::HasComponent<ComponentType>())
            {
                std::cout << "GameObject: " << m_Name << ", AddComponent: Can't add the same component twice\n";
                return;
            }

            m_Components.emplace_back(std::make_unique<ComponentType>(std::forward<Args>(args)...));
        }


        template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
            !std::is_same_v<Component, ComponentType>>>
            void RemoveComponent()
        {
            // if it does not have the component, you can't remove it
            if (!GameObject::HasComponent<ComponentType>())
            {
                std::cout << "GameObject: " << m_Name << ", RemoveComponent: No Component specified found\n";
                return;
            }


            auto it = std::ranges::find_if(m_Components,
                [](const auto& component)
                {
                    return dynamic_cast<ComponentType*>(component.get()) != nullptr;
                });

            m_Components.erase(it);
        }


        template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
            !std::is_same_v<Component, ComponentType>>>
            bool HasComponent() const
        {
            // checks if any of them have the same type a component is the
            // components vector
            return std::ranges::any_of(m_Components,
                [](const auto& component)
                {
                    return (dynamic_cast<ComponentType*>(component.get()) != nullptr);
                });

        }

        template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
            !std::is_same_v<Component, ComponentType>>>
            ComponentType* GetComponent() const
        {
            auto it = std::ranges::find_if(m_Components,
                [](const auto& component)
                {
                    return dynamic_cast<ComponentType*>(component.get()) != nullptr;
                });

            if (it == m_Components.end())
                return nullptr;

            return dynamic_cast<ComponentType*>(it->get());
        }

        std::vector<std::unique_ptr<Component>>& GetComponents();


    private:

        // DON'T USE THIS,
        // IT BREAK REALITY (AND YOUR MIND)
        void ForceDestroy();

        std::string m_Name{ "DefaultObject" };
        bool m_MarkedForDeletion{ false };

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children;
        std::vector<std::unique_ptr<Component>> m_Components;


    protected:
        void SetName(std::string newName);

        TransformComponent* m_Transform{ };


    };

}


