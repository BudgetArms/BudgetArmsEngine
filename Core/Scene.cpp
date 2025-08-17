#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "Components/Component.h"

using namespace bae;


Scene::Scene(const std::string& name) :
	m_Name(name)
{
}

Scene::~Scene()
{
	for (auto& object : m_Objects)
		object->Destroy();
};


void Scene::Update()
{
	for (auto& object : m_Objects)
		object->Update();

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

	m_Objects.erase(
		std::remove_if(m_Objects.begin(), m_Objects.end(),
			[](const std::shared_ptr<bae::GameObject>& uObject)
			{
				return uObject->IsMarkedForDeletion();
			}),
		m_Objects.end());

	for (auto& object : m_ObjectsPendingAdd)
		m_Objects.emplace_back(std::move(object));

	if (!m_ObjectsPendingAdd.empty())
		m_ObjectsPendingAdd.clear();


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


void Scene::Add(std::shared_ptr<GameObject> uObject)
{
	m_ObjectsPendingAdd.emplace_back(std::move(uObject));
}

void Scene::RemoveAll()
{
	for (auto& uObject : m_Objects)
		if (uObject)
			uObject->Destroy();
}


void Scene::Remove(std::shared_ptr<GameObject> uObject)
{
	std::erase(m_Objects, uObject);
}


