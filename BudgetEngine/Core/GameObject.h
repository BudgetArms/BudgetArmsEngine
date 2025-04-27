#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#include "Transform.h"
#include "Components/Component.h"


//bool bae::GameObject::m_bProjectClosing = false;

namespace bae
{
    class Texture2D;

    class GameObject
    {
    public:

        //GameObject() = default;
        GameObject()
        {
            //atexit(SetProjectClosing);
            //atexit((void)Test);
        };

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


        std::string GetName() const { return m_Name; };
        // made for serialization
        void SetName(std::string newName);


        GameObject* GetParent() const;
        void SetParent(GameObject* newParent, bool keepLocation);


#pragma region Components

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
        void AddComponent(T)
        {
            // We don't need two components of the same type
            if (GameObject::HasComponent<T>())
                return;

            m_Components.push_back(std::make_unique<T>());
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
        void RemoveComponent(T)
        {
            // if it does not have the component, you can't remove it
            if (!GameObject::HasComponent<T>())
                return;

            m_Components.erase(GameObject::GetComponent<T>());
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        bool HasComponent(T) const
        {
            // checks if any of them have the same type a component is the 
            // components vector
            return std::any_of(m_Components,
                [](const auto& component) {
                    return (dynamic_cast<T*>(component.get()) != nullptr);
                });

        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
        T* GetComponent() const
        {
            return std::find(m_Components,
                [](const auto& component)
                {
                    return dynamic_cast<T*>(component.get());
                });
        }

        std::vector<std::unique_ptr<Component>>& GetComponents()
        {
            return m_Components;
        }

#pragma endregion 


        void SetTexture(const std::string& filename);
        void SetPosition(float x, float y);

        void SetLocalPosition(const glm::vec3& pos);

        static void SetProjectClosing() { m_bProjectClosing = true; }


        static bool m_bProjectClosing;


    private:

        void AddChild(GameObject* child, bool updateLocation = true);
        // AKA DESTROY CHILD, DOESN'T GIVE A FCK ABOUT RAII
        void RemoveChild(GameObject* child, bool keepChildrenOfChild = false, bool updateChildrenOfChildLocations = true);

        bool IsChild(const GameObject* child) const;


        const glm::vec3& GetWorldPosition();
        void UpdateWorldPosition();

        void SetPositionDirty() { m_PositionDirty = true; }

        void ForceDestroy();


        std::string m_Name{ "DefaultObject" };

        Transform m_Transform{};
        std::shared_ptr<Texture2D> m_Texture{};

        glm::vec3 m_LocalPosition{};
        glm::vec3 m_WorldPosition{};
        bool m_PositionDirty{ true };


        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children;
        std::vector<std::unique_ptr<Component>> m_Components;

        bool m_MarkedForDeletion{ false };


    };
}

