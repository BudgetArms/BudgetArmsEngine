#include "Scene.hpp"
#include "GameObject.hpp"

#include <algorithm>


using namespace bae;


Scene::~Scene()
{
    for(const std::shared_ptr<GameObject>& object : m_Objects)
    {
        object->Destroy();
    }

    m_Objects.clear();
}

void Scene::Update() const
{
    for(const std::shared_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->Update();
        }
    }
}


void Scene::FixedUpdate() const
{
    for(const std::shared_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->FixedUpdate();
        }
    }
}

void Scene::LateUpdate()
{
    for(const std::shared_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->LateUpdate();
        }
    }

    std::erase_if(m_Objects,
                  [](const std::shared_ptr<GameObject>& uObject)
                  {
                      return uObject->IsMarkedForDeletion();
                  });

    for(auto sObject : m_ObjectsPendingAdd)
    {
        if(sObject)
        {
            m_Objects.emplace_back(std::move(sObject));
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
    for(const std::shared_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->Render();
        }
    }
}

void Scene::RenderGUI() const
{
    for(const std::shared_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->RenderGUI();
        }
    }
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
    m_ObjectsPendingAdd.emplace_back(std::move(object));
}


void Scene::RemoveAll() const
{
    for(const std::shared_ptr<GameObject>& sObject : m_Objects)
    {
        if(sObject)
        {
            sObject->Destroy();
        }
    }
}

Scene::Scene(const std::string& name) :
    m_Name(name)
{
}


void Scene::Remove(const std::shared_ptr<GameObject>& object)
{
    std::erase(m_Objects, object);
}


