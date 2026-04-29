#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Singletons/Singleton.h"


namespace bae
{
    class Scene;

    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        Scene& CreateScene(const std::string& name);

        void Update() const;
        void FixedUpdate() const;
        void LateUpdate() const;
        void Render() const;
        void RenderGUI() const;

    private:
        friend class Singleton;
        SceneManager() = default;

        std::vector<std::shared_ptr<Scene>> m_Scenes;
    };
}

