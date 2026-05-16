#include "SceneManager.hpp"

#include "Core/Scene.hpp"


bae::Scene& bae::SceneManager::CreateScene(const std::string& name)
{
    const auto& scene = std::shared_ptr<Scene>(new Scene(name));
    m_Scenes.push_back(scene);

    return *scene;
}

void bae::SceneManager::Update() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled)
        {
            scene->Update();
        }
    }
}

void bae::SceneManager::FixedUpdate() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled)
        {
            scene->FixedUpdate();
        }
    }
}

void bae::SceneManager::LateUpdate() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled)
        {
            scene->LateUpdate();
        }
    }
}

void bae::SceneManager::Render() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled)
        {
            scene->Render();
        }
    }
}


void bae::SceneManager::RenderGUI() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled)
        {
            scene->RenderGUI();
        }
    }
}


