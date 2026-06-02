#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Singletons/Singleton.hpp"


namespace bae
{
    class Scene;

    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        void Destroy();

        Scene& CreateScene(const std::string& sceneName);
        bool MarkSceneForDestruction(const std::string& sceneName);

        [[nodiscard]] Scene* GetScene(const std::string& sceneName);

        void Update() const;
        void FixedUpdate() const;
        void LateUpdate();
        void Render() const;
        void RenderGUI() const;

    private:
        friend class Singleton;
        SceneManager() = default;

        void DestroyScene(const std::string& sceneName);


        std::vector<std::shared_ptr<Scene>> m_Scenes{};
        std::set<std::string> m_SceneNamesToBeDestroyed{};
    };
}

