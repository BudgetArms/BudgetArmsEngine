#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Core/GameObject.h"
#include "Managers/SceneManager.h"


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

        void Add(std::shared_ptr<GameObject> object);
        void RemoveAll() const;

        std::string GetName() const { return m_Name; }
        std::vector<std::shared_ptr<GameObject>>& GetObjects() { return m_Objects; };


        bool m_bIsEnabled{ true };

    private:
        explicit Scene(const std::string& name);

        friend Scene& SceneManager::CreateScene(const std::string& name);
        void Remove(const std::shared_ptr<GameObject>& object);


        std::string m_Name;
        std::vector<std::shared_ptr<GameObject>> m_Objects{};
        std::vector<std::shared_ptr<GameObject>> m_ObjectsPendingAdd{};
    };
}

