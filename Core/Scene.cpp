#include "Scene.hpp"
#include "GameObject.hpp"

#include <algorithm>


using namespace bae;


Scene::~Scene()
{
    for(const std::unique_ptr<GameObject>& uObject : m_Objects)
    {
        uObject->Destroy();
    }

    m_Objects.clear();
}

void Scene::Update() const
{
    for(const std::unique_ptr<GameObject>& uObject : m_Objects)
    {
        if(uObject)
        {
            uObject->Update();
        }
    }
}


void Scene::FixedUpdate() const
{
    for(const std::unique_ptr<GameObject>& uObject : m_Objects)
    {
        if(uObject)
        {
            uObject->FixedUpdate();
        }
    }
}

void Scene::LateUpdate()
{
    for(const std::unique_ptr<GameObject>& uObject : m_Objects)
    {
        if(uObject)
        {
            uObject->LateUpdate();
        }
    }

    // Destroy Objects marked for deletion
    std::erase_if(m_Objects, [](const std::unique_ptr<GameObject>& uObject)
    {
        return uObject->IsMarkedForDeletion();
    });

    for(auto& uObject : m_ObjectsPendingAdd)
    {
        if(uObject)
        {
            m_Objects.emplace_back(std::move(uObject));
        }
    }

    // Clear for safety
    if(!m_ObjectsPendingAdd.empty())
    {
        m_ObjectsPendingAdd.clear();
    }
}

void Scene::Render() const
{
    for(const std::unique_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->Render();
        }
    }
}

void Scene::RenderGUI() const
{
    for(const std::unique_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->RenderGUI();
        }
    }
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
    m_ObjectsPendingAdd.emplace_back(std::move(object));
}


void Scene::RemoveAll() const
{
    for(const std::unique_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->Destroy();
        }
    }
}

void Scene::ForceRemoveAll()
{
    for(const auto& object : m_Objects)
    {
        object->Destroy();
    }

    m_Objects.clear();
}

Scene::Scene(const std::string& name) :
    m_Name(name)
{
}


void Scene::Remove(const std::unique_ptr<GameObject>& object)
{
    std::erase(m_Objects, object);
}


