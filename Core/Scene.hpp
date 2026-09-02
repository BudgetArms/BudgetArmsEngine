#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Core/GameObject.hpp"
#include "Managers/SceneManager.hpp"


namespace bae
{
    class GameObject;

    class Scene final
    {
    public:
        ~Scene();

        Scene(const Scene& other)            = delete;
        Scene(Scene&& other)                 = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other)      = delete;


        void Update() const;
        void FixedUpdate() const;
        void LateUpdate();
        void Render() const;
        void RenderGUI() const;

        void Add(std::unique_ptr<GameObject> object);
        void RemoveAll() const;

        // This will crash your game if used incorrectly
        void ForceRemoveAll();

        [[nodiscard]] std::string GetName() const;
        std::vector<std::unique_ptr<GameObject>>& GetObjects();

        void Destroy();
        [[nodiscard]] bool IsMarkedForDeletion() const;


        bool m_bIsEnabled{ true };

    private:
        explicit Scene(const std::string& name);

        friend Scene& SceneManager::CreateScene(const std::string& name);
        void Remove(const std::unique_ptr<GameObject>& object);


        std::string m_Name{ "Default" };
        std::vector<std::unique_ptr<GameObject>> m_Objects{};
        std::vector<std::unique_ptr<GameObject>> m_ObjectsPendingAdd{};

        bool m_bIsMarkedForDeletion{};
    };
}

