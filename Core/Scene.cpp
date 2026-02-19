#include "Scene.h"
#include "GameObject.h"

#include <algorithm>


using namespace bae;


Scene::~Scene()
{
    for (auto& object : m_Objects)
        object->Destroy();
}

Scene::Scene(const std::string& name) :
    m_Name(name)
{
};


void Scene::Update()
{
	for (auto sObject : m_Objects)
	    if (sObject)
		    sObject->Update();

}

void Scene::FixedUpdate()
{
    for (auto sObject : m_Objects)
	    if (sObject)
		    sObject->FixedUpdate();

}

void bae::Scene::LateUpdate()
{
     for (auto sObject : m_Objects)
	    if (sObject)
		    sObject->LateUpdate();

	std::erase_if(m_Objects,
	    [](const std::shared_ptr<bae::GameObject>& uObject)
	    {
	        return uObject->IsMarkedForDeletion();
	    });

    for (auto sObject : m_ObjectsPendingAdd)
	    if (sObject)
	        m_Objects.emplace_back(std::move(sObject));

    // Clear for safety
	if (!m_ObjectsPendingAdd.empty())
		m_ObjectsPendingAdd.clear();


}

void Scene::Render() const
{
    for (auto sObject : m_Objects)
	    if (sObject)
	        sObject->Render();

}

void Scene::RenderGUI()
{
    for (auto sObject : m_Objects)
	    if (sObject)
	        sObject->RenderGUI();

}


void Scene::Add(std::shared_ptr<GameObject> sObject)
{
	m_ObjectsPendingAdd.emplace_back(std::move(sObject));
}

void Scene::RemoveAll()
{
	for (auto& sObject : m_Objects)
		if (sObject)
			sObject->Destroy();
}


void Scene::Remove(std::shared_ptr<GameObject> sObject)
{
	std::erase(m_Objects, sObject);
}


