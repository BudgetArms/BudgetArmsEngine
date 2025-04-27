#include <string>
#include <iostream>
#include "GameObject.h"
#include "Managers/ResourceManager.h"
#include "Renderer.h"
#include "glm.hpp"

//#include "Components/RenderComponent.h"


// BudgetArmsEngine
using namespace bae;

bool GameObject::m_bProjectClosing = false;

GameObject::~GameObject()
{
    // if it gets deleted with being marked for deletion && it's the project closing; assert
    if (!m_MarkedForDeletion && !m_bProjectClosing)
    {
        std::cout << "GameObject::Destructor, Object should not be destroyed, because it's not marked for destruction\n";
        assert(false && "GameObject::Destructor, Object should not be destroyed, because it's not marked for destruction");
    }

    // remove child forcefully remove the children.
    // But technically speaking, GameObject should not remove other GameObjects :/
    for (auto& child : m_Children)
        RemoveChild(child, false);

}

void GameObject::Update()
{
    for (auto& child : m_Children)
        child->Update();

    for (auto& component : m_Components)
        component->Update();

}

void GameObject::FixedUpdate()
{
    // The Physics/Networking gets done, by the Components and/or the future GameObjects
    for (auto& child : m_Children)
        child->FixedUpdate();

    for (auto& component : m_Components)
        component->FixedUpdate();

}

void GameObject::LateUpdate()
{
    // Check if we need to self-destruct
    if (m_MarkedForDeletion)
    {
        // destroy children && the children of children
        for (auto& child : m_Children)
            RemoveChild(child, false);

        delete(this);
    }
    else
    {
        // the object should not have been destroyed
    }
}



void GameObject::Render() const
{
    const glm::vec3& pos = m_Transform.GetPosition();
    Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);

    //for (auto& component : m_Components)
        //if (auto renderComp = dynamic_cast<RenderComponent*>(component.get()))
            //renderComp->Render();

}

void GameObject::Destroy()
{
    m_MarkedForDeletion = true;

    for (auto& child : m_Children)
        child->Destroy();
}

// this is for serialization, just as for all inputs
void GameObject::SetName(std::string newName)
{
    m_Name = newName;
}


GameObject* GameObject::GetParent() const
{
    return m_Parent;
}


void GameObject::SetParent(GameObject* newParent, bool keepLocation = true)
{
    // if invalid
    if (this == newParent || m_Parent == newParent || IsChild(newParent))
        return;

    std::cout << "(GameObject)newParent is not itself/Parent or Children" << '\n';

    if (newParent == nullptr)
        SetLocalPosition(GetWorldPosition());
    else
    {
        // if keeplocation, localpos with adjust to newParent's worldlocation
        // else, localPos (eg {100, 0, 0}, will also be {100, 0, 0} compared to newTarget 
        // We could also have an option to reset LocalPos always to {0, 0, 0}, but why?
        if (keepLocation)
            SetLocalPosition(GetWorldPosition() - newParent->GetWorldPosition());

        SetPositionDirty();
    }


    if (m_Parent)
        m_Parent->RemoveChild(this, true, true);

    m_Parent = newParent;

    if (m_Parent != nullptr)
        //m_Parent->AddChild(this, true);
        m_Parent->AddChild(this);

}


void GameObject::AddChild(GameObject* newChild, bool updateLocation)
{
    // if invalid
    if (this == newChild || m_Parent == newChild || newChild == nullptr || IsChild(newChild))
        return;

    std::cout << "(GameObject)newChild is not itself/Parent or Children" << '\n';


    if (updateLocation)
        newChild->SetLocalPosition(newChild->GetWorldPosition() - GetWorldPosition());

    SetPositionDirty();


    // this removes the child from the parent's children list
    if (newChild->m_Parent)
        std::erase(newChild->m_Parent->m_Children, newChild);

    // Make myself parent from newChild (get adopted) :D
    newChild->m_Parent = this;

    // YOU ARE NOW MY CHILD!!!
    m_Children.emplace_back(newChild);

}



void GameObject::RemoveChild(GameObject* deleteChild, bool keepChildrenOfChild, bool updateChildrenOfChildLocations)
{
    // if invalid
    if (this == deleteChild || m_Parent == deleteChild || deleteChild == nullptr || !IsChild(deleteChild))
    {
        std::cout << "(GameObject) deleteChild is itself/Parent or not in m_Children" << '\n';
        return;
    }


    if (updateChildrenOfChildLocations)
        deleteChild->SetLocalPosition(GetWorldPosition());

    deleteChild->SetPositionDirty();


    // remove deleteChild from my child list
    std::erase(m_Children, deleteChild);

    if (!keepChildrenOfChild)
    {
        // delete deleteChild's children
        //deleteChild->m_Children.clear();

        // removing and then destroying the children
        for (auto& child : m_Children)
            RemoveChild(child, false);

    }


    deleteChild->m_Parent = nullptr;

    // kill the child
    delete(deleteChild);


}


bool GameObject::IsChild(const GameObject* child) const
{
    return std::ranges::find(m_Children, child) != m_Children.end();
}



void GameObject::SetTexture(const std::string& filename)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}


void GameObject::SetPosition(float x, float y)
{
    m_Transform.SetPosition(x, y, 0.0f);
}




void GameObject::SetLocalPosition(const glm::vec3& pos)
{
    m_LocalPosition = pos;
    SetPositionDirty();
}

const glm::vec3& GameObject::GetWorldPosition()
{
    if (m_PositionDirty)
        UpdateWorldPosition();

    return m_WorldPosition;
}


void GameObject::UpdateWorldPosition()
{
    if (m_PositionDirty)
    {
        if (m_Parent == nullptr)
            m_WorldPosition = m_LocalPosition;
        else
            m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
    }

    m_PositionDirty = false;
}


void bae::GameObject::ForceDestroy()
{


}


