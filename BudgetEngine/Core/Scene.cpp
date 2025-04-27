#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "Components/Component.h"
//#include "InputComponent.h"

// BudgetArmsEngine
using namespace bae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
    m_Objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
    // since C++ 20
    std::erase(m_Objects, object);

    // pre C++ 20
    //m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
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


