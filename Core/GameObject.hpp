#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>


namespace bae
{
    class Component;
    class TransformComponent;

    class GameObject final
    {
    public:
        explicit GameObject(const std::string& name);
        ~GameObject();

        GameObject(const GameObject&)            = delete;
        GameObject(GameObject&&)                 = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject& operator=(GameObject&&)      = delete;


        void Update() const;
        void FixedUpdate() const;
        void LateUpdate() const;
        void Render() const;
        void RenderGUI() const;


        // Mark for destruction
        void Destroy();

        // this is a way more accurate name, Remove Child sounds too much like destroying
        void AttachChild(GameObject* child, bool bFreezeLocation = true, bool bFreezeRotation = true,
                         bool bFreezeScale                       = true);
        void DetachChild(GameObject* child, bool bUpdateChildrenOfChildLocations = true);

        bool IsChild(const GameObject* child) const;

        [[nodiscard]] std::string GetName() const { return m_Name; }

        [[nodiscard]] GameObject* GetParent() const;
        void SetParent(GameObject* newParent, bool bKeepLocation);

        [[nodiscard]] constexpr bool IsMarkedForDeletion() const { return m_MarkedForDeletion; }


        [[nodiscard]] const glm::vec2& GetWorldLocation() const;
        [[nodiscard]] float GetWorldRotation() const;
        [[nodiscard]] const glm::vec2& GetWorldScale() const;

        [[nodiscard]] const glm::vec2& GetLocalLocation() const;
        [[nodiscard]] float GetLocalRotation() const;
        [[nodiscard]] const glm::vec2& GetLocalScale() const;

        void SetWorldLocation(const glm::vec2& location) const;
        void SetWorldRotation(float rotation) const;
        void SetWorldScale(const glm::vec2& scale) const;

        void SetLocalLocation(const glm::vec2& location) const;
        void SetLocalRotation(float rotation) const;
        void SetLocalScale(const glm::vec2& scale) const;

        void AddLocation(const glm::vec2& addLocation) const;
        void AddRotation(float addRotation) const;
        void AddScale(const glm::vec2& addScale) const;

        void SetLocationDirty() const;
        void SetRotationDirty() const;
        void SetScaleDirty() const;


        std::vector<std::unique_ptr<Component>>& GetComponents()
        {
            return m_Components;
        }


        template<typename ComponentType, typename... Args,
                 typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
                     !std::is_same_v<Component, ComponentType>>>
        void AddComponent(Args&&... args)
        {
            // We don't need two components of the same type
            if(GameObject::HasComponent<ComponentType>())
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
            if(!GameObject::HasComponent<ComponentType>())
            {
                std::cout << "GameObject: " << m_Name << ", RemoveComponent: No Component specified found\n";
                return;
            }


            auto it = std::ranges::find_if(m_Components, [](const auto& component)
            {
                return dynamic_cast<ComponentType*>(component.get()) != nullptr;
            });

            m_Components.erase(it);
        }


        template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
                     !std::is_same_v<Component, ComponentType>>>
        [[nodiscard]] bool HasComponent() const
        {
            // checks if any of them have the same type a component is the
            // components vector
            return std::ranges::any_of(m_Components, [](const auto& component)
            {
                return dynamic_cast<ComponentType*>(component.get()) != nullptr;
            });
        }

        template<typename ComponentType, typename = std::enable_if_t<std::is_base_of_v<Component, ComponentType> &&
                     !std::is_same_v<Component, ComponentType>>>
        [[nodiscard]] ComponentType* GetComponent() const
        {
            auto it = std::ranges::find_if(m_Components, [](const auto& component)
            {
                return dynamic_cast<ComponentType*>(component.get()) != nullptr;
            });

            if(it == m_Components.end())
            {
                return nullptr;
            }

            return dynamic_cast<ComponentType*>(it->get());
        }

    private:
        // DON'T USE THIS UNLESS ABSOLUTELY NECESSARY (THIS WILL MOST LIKELY RESULT IN A CRASH)
        void ForceDestroy();

        std::string m_Name{ "DefaultObject" };

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children;
        std::vector<std::unique_ptr<Component>> m_Components;

        bool m_MarkedForDeletion{ false };

    protected:
        void SetName(const std::string& newName);

        TransformComponent* m_Transform{};
    };
}


