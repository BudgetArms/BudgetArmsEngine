#include <string>
#include <iostream>
#include "GameObject.h"
#include "Managers/ResourceManager.h"
#include "Core/Renderer.h"


#include "Components/TransformComponent.h"
#include "Components/Component.h"



// BudgetArmsEngine
using namespace bae;


GameObject::GameObject(const std::string& name)
{
    SetName(name);

    AddComponent<TransformComponent>(*this);
    m_Transform = GetComponent<TransformComponent>();
}

GameObject::~GameObject()
{
    // my design philosophy:
    // Shared pointers: reset in LateUpdate if Marked For Deletion
    // Unique pointers: release in LateUpdate if Marked For Deletion

    // No Object/class should destroy itself.


    // if it gets deleted with being marked for deletion && it's the project closing; assert
    if (!m_MarkedForDeletion)
    {
        const std::string errorMessage{ "GameObject: " + m_Name + ", Destructor, Object should not be destroyed, because it's not marked for destruction\n" };
        std::cout << errorMessage;
        assert(false && errorMessage.c_str());
    }

}

void GameObject::Update()
{
    // Every game object should be in the scene, so no need update the children
    // from gameobject, you learn every day; apply that knowledge
    //for (auto& child : m_Children)
        //child->Update();

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
    for (auto& component : m_Components)
        component->LateUpdate();
}

void GameObject::Render() const
{
    for (auto& component : m_Components)
        component->Render();
}

void GameObject::RenderGUI()
{
    for (auto& component : m_Components)
        component->RenderGUI();
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
    {
        std::cout << "GameObject: " << m_Name << ", newParent is invalid\n";
        return;
    }


    if (newParent == nullptr)
    {
        SetLocalLocation(GetWorldLocation());
    }
    else
    {
        // if keeplocation, localpos with adjust to newParent's worldlocation
        // else, localPos (eg {100, 0, 0}, will also be {100, 0, 0} compared to newTarget 
        // We could also have an option to reset LocalPos always to {0, 0, 0}, but why?
        if (keepLocation)
            SetLocalLocation(GetWorldLocation() - newParent->GetWorldLocation());

        SetLocationDirty();
    }


    if (m_Parent)
        m_Parent->AttachChild(this, true);

    m_Parent = newParent;

    if (m_Parent != nullptr)
        m_Parent->AttachChild(this);

}

void GameObject::AttachChild(GameObject* newChild, bool freezeLocation, bool freezeRotation, bool freezeScale)
{
    // if invalid
    if (this == newChild || m_Parent == newChild || newChild == nullptr || IsChild(newChild))
    {
        std::cout << "GameObject: " << m_Name << ", AttachChild: newChild is invalid\n";
        return;
    }

    if (freezeLocation)
        newChild->SetLocalLocation(newChild->GetWorldLocation() - GetWorldLocation());

    if (freezeRotation)
        newChild->SetLocalRotation(newChild->GetWorldRotation() - GetWorldRotation());

    if (freezeScale)
        newChild->SetLocalScale(newChild->GetWorldScale() / GetWorldScale());


    SetLocationDirty();
    SetRotationDirty();
    SetScaleDirty();
    newChild->SetLocationDirty();
    newChild->SetRotationDirty();
    newChild->SetScaleDirty();


    // this removes the child from the parent's children list
    if (newChild->m_Parent)
        std::erase(newChild->m_Parent->m_Children, newChild);

    // Make myself parent from newChild (get adopted) :D
    newChild->m_Parent = this;

    // YOU ARE NOW MY CHILD!!!
    m_Children.emplace_back(newChild);

}

void GameObject::DetachChild(GameObject* deleteChild, bool updateChildLocation)
{
    // if invalid
    if (this == deleteChild || m_Parent == deleteChild || deleteChild == nullptr || !IsChild(deleteChild))
    {
        std::cout << "GameObject: " << m_Name << ", DetachChild: deleteChild is invalid\n";
        return;
    }


    if (updateChildLocation)
        deleteChild->SetLocalLocation(GetWorldLocation());

    deleteChild->SetLocationDirty();
    deleteChild->SetRotationDirty();
    deleteChild->SetScaleDirty();


    // remove deleteChild from my child list
    std::erase(m_Children, deleteChild);

    deleteChild->m_Parent = nullptr;

}

bool GameObject::IsChild(const GameObject* child) const
{
    return std::ranges::find(m_Children, child) != m_Children.end();
}

void GameObject::ForceDestroy()
{
    // Go DFS (Deep First Search)
    for (auto& child : m_Children)
        child->ForceDestroy();

    for (auto& child : m_Children)
        if (child)
            std::cout << "child should be dead?\n";


    m_Children.clear();
    m_MarkedForDeletion = true;


    // fuck it, let's crash
    delete(this);
}


#pragma region Tranform Functions



const glm::vec3& GameObject::GetWorldLocation()
{
    return m_Transform->GetWorldLocation();
}

float GameObject::GetWorldRotation()
{
    return m_Transform->GetWorldRotation();
}

const glm::vec2& GameObject::GetWorldScale()
{
    return m_Transform->GetWorldScale();
}



const glm::vec3& GameObject::GetLocalLocation() const
{
    return m_Transform->GetLocalLocation();
}

float GameObject::GetLocalRotation() const
{
    return m_Transform->GetLocalRotation();
}

const glm::vec2& GameObject::GetLocalScale() const
{
    return m_Transform->GetLocalScale();
}



void GameObject::SetWorldLocation(const glm::vec3& location)
{
    m_Transform->SetWorldLocation(location);
}

void GameObject::SetWorldRotation(float rotation)
{
    m_Transform->SetWorldRotation(rotation);
}

void GameObject::SetWorldScale(const glm::vec2& scale)
{
    m_Transform->SetWorldScale(scale);
}



void GameObject::SetLocalLocation(const glm::vec3& location)
{
    m_Transform->SetLocalLocation(location);
}

void GameObject::SetLocalRotation(float rotation)
{
    m_Transform->SetLocalRotation(rotation);
}

void GameObject::SetLocalScale(const glm::vec2& scale)
{
    m_Transform->SetLocalScale(scale);
}



void GameObject::AddLocation(const glm::vec3& addLocation)
{
    m_Transform->AddLocation(addLocation);
}

void GameObject::AddRotation(float addRotation)
{
    m_Transform->AddRotation(addRotation);
}

void GameObject::AddScale(const glm::vec2& addScale)
{
    m_Transform->AddScale(addScale);
}



constexpr void GameObject::SetLocationDirty()
{
    m_Transform->SetLocationDirty();
}

constexpr void GameObject::SetRotationDirty()
{
    m_Transform->SetRotationDirty();
}

constexpr void GameObject::SetScaleDirty()
{
    m_Transform->SetScaleDirty();
}



#pragma endregion


