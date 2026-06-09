#include "SceneManager.hpp"

#include "Core/HelperFunctions.hpp"
#include "Core/Scene.hpp"


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

void bae::SceneManager::LateUpdate()
{
    for(const auto& scene : m_Scenes)
    {
        if(scene->m_bIsEnabled)
        {
            scene->LateUpdate();
        }
    }

    // Destroy Marked scenes
    for(const std::string& nameScene : m_SceneNamesToBeDestroyed)
    {
        DestroyScene(nameScene);
    }

    m_SceneNamesToBeDestroyed.clear();
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
    m_Scenes.push_back(scene);

    return *scene;
}

bool bae::SceneManager::MarkSceneForDestruction(const std::string& sceneName)
{
    const size_t oldToBeDestroyedSize = m_SceneNamesToBeDestroyed.size();

    for(const auto& scene : m_Scenes)
    {
        if(scene->GetName() == sceneName)
        {
            m_SceneNamesToBeDestroyed.insert(sceneName);
        }
    }

    const size_t newToBeDestroyedSize = m_SceneNamesToBeDestroyed.size();

    if(oldToBeDestroyedSize == newToBeDestroyedSize)
    {
        return false;
    }

    return true;
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


void bae::SceneManager::DestroyScene(const std::string& name)
{
    const Scene* scene = GetScene(name);
    if(!scene)
    {
        std::cout << FUNCTION_NAME << " Failed to find scene!" << '\n';
        return;
    }


    const size_t amountScenesRemoved = std::erase_if(m_Scenes, [&](const auto& lambdaScene)
    {
        return lambdaScene->GetName() == name;
    });

    if(amountScenesRemoved <= 0)
    {
        std::cout << FUNCTION_NAME << " Failed To Destroy Scene, Somehow Couldn't Be Erased!" << '\n';
    }
}


