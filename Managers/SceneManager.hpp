#pragma once

#include <memory>
#include <set>
#include <string>

#include "Singletons/Singleton.hpp"


namespace bae
{
    class Scene;

    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        void Update() const;
        void FixedUpdate() const;
        void LateUpdate();
        void Render() const;
        void RenderGUI() const;

        void Destroy();

        Scene& CreateScene(const std::string& sceneName);

        [[nodiscard]] Scene* GetScene(const std::string& sceneName);

        [[nodiscard]] bool IsDestroyed() const;

    private:
        friend class Singleton;
        SceneManager() = default;
        ~SceneManager() override;


        std::set<std::shared_ptr<Scene>> m_Scenes{};

        bool m_bIsDestroyed{};
    };
}

