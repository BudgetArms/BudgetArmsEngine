#include "SceneManager.hpp"

#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"


void bae::SceneManager::Update() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled && !scene->IsMarkedForDeletion())
        {
            scene->Update();
        }
    }
}

void bae::SceneManager::FixedUpdate() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled && !scene->IsMarkedForDeletion())
        {
            scene->FixedUpdate();
        }
    }
}

void bae::SceneManager::LateUpdate()
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled && !scene->IsMarkedForDeletion())
        {
            scene->LateUpdate();
        }
    }

    // Destroy Marked scenes
    std::erase_if(m_Scenes, [](const std::shared_ptr<Scene>& scene)
    {
        return scene->IsMarkedForDeletion();
    });
}

void bae::SceneManager::Render() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled && !scene->IsMarkedForDeletion())
        {
            scene->Render();
        }
    }
}

void bae::SceneManager::RenderGUI() const
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled && !scene->IsMarkedForDeletion())
        {
            scene->RenderGUI();
        }
    }
}

void bae::SceneManager::Destroy()
{
    m_bIsDestroyed = true;
    m_Scenes.clear();
}


bae::Scene& bae::SceneManager::CreateScene(const std::string& name)
{
    // if scene already exists
    if(Scene* scene = GetScene(name))
    {
        return *scene;
    }

    const auto& scene = std::shared_ptr<Scene>(new Scene(name));
    m_Scenes.insert(scene);

    return *scene;
}


bae::Scene* bae::SceneManager::GetScene(const std::string& name)
{
    if(m_bIsDestroyed)
    {
        return nullptr;
    }

    const auto sceneIt = std::ranges::find_if(m_Scenes, [&name](const auto& scene)
    {
        if(!scene)
        {
            return false;
        }

        return scene->GetName() == name;
    });

    if(sceneIt == m_Scenes.end() || !sceneIt->get())
    {
        return nullptr;
    }

    return sceneIt->get();
}

bool bae::SceneManager::IsDestroyed() const
{
    return m_bIsDestroyed;
}

bae::SceneManager::~SceneManager()
{
    if(!m_bIsDestroyed)
    {
        const std::string errorMessage = std::string(FUNCTION_NAME) +
                " Failed! Destroy should be called before program close";

        std::cout << errorMessage << '\n';
        assert(false && errorMessage.c_str());
    }
}
