#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "Components/Component.h"
//#include "InputComponent.h"

using namespace bae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) :
    m_Name(name)
{
}

Scene::~Scene()
{
    for (auto& object : m_Objects)
        object->Destroy();
};

void Scene::Add(std::shared_ptr<GameObject> object)
{
    m_Objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
    // since C++ 20
    std::erase(m_Objects, object);
}

void Scene::RemoveAll()
{
    m_Objects.clear();
}

void Scene::FixedUpdate()
{
    for (auto& object : m_Objects)
        object->FixedUpdate();

}

void bae::Scene::LateUpdate()
{
    for (auto& object : m_Objects)
        object->LateUpdate();

    for (auto& object : m_Objects)
        if (object->IsMarkedForDeletion())
            Remove(object);

}

void Scene::Update()
{
    for (auto& object : m_Objects)
        object->Update();

}

void Scene::Render() const
{
    for (const auto& object : m_Objects)
        object->Render();

}

void Scene::RenderGUI()
{
    for (const auto& object : m_Objects)
        object->RenderGUI();

}


