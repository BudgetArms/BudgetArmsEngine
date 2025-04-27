#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Managers/SceneManager.h"
#include "GameObject.h"

namespace bae
{
    class GameObject;
    class Scene final
    {
        friend Scene& SceneManager::CreateScene(const std::string& name);
    public:
        void Add(std::shared_ptr<GameObject> object);
        void Remove(std::shared_ptr<GameObject> object);
        void RemoveAll();

        void Update();
        void FixedUpdate();
        void LateUpdate();
        void Render() const;

        ~Scene();
        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other) = delete;

    private:
        explicit Scene(const std::string& name);

        std::string m_Name;
        std::vector<std::shared_ptr<GameObject>> m_Objects{};

        static unsigned int m_IdCounter;
    };

}
