#include "GameObject.hpp"

#include <cassert>
#include <iostream>
#include <string>

#include "HelperFunctions.hpp"
#include "Components/Component.hpp"
#include "Components/TransformComponent.hpp"
#include "Core/Renderer.hpp"


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
    if(!m_MarkedForDeletion)
    {
        const std::string errorMessage = std::string(FUNCTION_NAME) + " Failed to Delete GameObject \"" + m_Name +
                "\" is not marked for destruction\n";

        std::cout << errorMessage;
        assert(false && errorMessage.c_str());
    }

    std::cout << FUNCTION_NAME << " Name: " << m_Name << '\n';
}

void GameObject::Update() const
{
    for(const GameObject* pChild : m_Children)
    {
        if(pChild)
        {
            pChild->Update();
        }
    }

    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(component)
        {
            component->Update();
        }
    }
}

void GameObject::FixedUpdate() const
{
    // The Physics/Networking gets done, by the Components and/or the future GameObjects
    for(const GameObject* pChild : m_Children)
    {
        if(pChild)
        {
            pChild->FixedUpdate();
        }
    }

    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(component)
        {
            component->FixedUpdate();
        }
    }
}

void GameObject::LateUpdate() const
{
    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(component)
        {
            component->LateUpdate();
        }
    }
}

void GameObject::Render() const
{
    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(component)
        {
            component->Render();
        }
    }
}

void GameObject::RenderGUI() const
{
    for(const auto& component : m_Components)
    {
        if(component)
        {
            component->RenderGUI();
        }
    }
}

void GameObject::Destroy()
{
    m_MarkedForDeletion = true;

    for(GameObject* pChild : m_Children)
    {
        if(pChild)
        {
            pChild->Destroy();
        }
    }
}


void GameObject::AttachChild(GameObject* child, const bool bFreezeLocation, const bool bFreezeRotation,
                             const bool bFreezeScale)
{
    // if invalid
    if(this == child || m_Parent == child || child == nullptr || IsChild(child))
    {
        std::cout << "GameObject: " << m_Name << ", AttachChild: newChild is invalid\n";
        return;
    }

    if(bFreezeLocation)
    {
        child->SetLocalLocation(child->GetWorldLocation() - GetWorldLocation());
    }

    if(bFreezeRotation)
    {
        child->SetLocalRotation(child->GetWorldRotation() - GetWorldRotation());
    }

    if(bFreezeScale)
    {
        child->SetLocalScale(child->GetWorldScale() / GetWorldScale());
    }

    child->SetLocationDirty();
    child->SetRotationDirty();
    child->SetScaleDirty();

    // this removes the child from the parent's children list
    if(child->m_Parent)
    {
        std::erase(child->m_Parent->m_Children, child);
    }

    // Make myself parent from newChild (get adopted) :D
    child->m_Parent = this;

    // YOU ARE NOW MY CHILD!!!
    m_Children.emplace_back(child);
}

void GameObject::DetachChild(GameObject* child, const bool bUpdateChildrenOfChildLocations)
{
    // if invalid
    if(this == child || m_Parent == child || child == nullptr || !IsChild(child))
    {
        std::cout << "GameObject: " << m_Name << ", DetachChild: deleteChild is invalid\n";
        return;
    }


    if(bUpdateChildrenOfChildLocations)
    {
        child->SetLocalLocation(GetWorldLocation());
    }

    child->SetLocationDirty();
    child->SetRotationDirty();
    child->SetScaleDirty();


    // remove deleteChild from my child list
    std::erase(m_Children, child);

    child->m_Parent = nullptr;
}

bool GameObject::IsChild(const GameObject* child) const
{
    return std::ranges::find(m_Children, child) != m_Children.end();
}

GameObject* GameObject::GetParent() const
{
    return m_Parent;
}

void GameObject::SetParent(GameObject* newParent, const bool bKeepLocation = true)
{
    // if invalid
    if(this == newParent || m_Parent == newParent || IsChild(newParent))
    {
        std::cout << "GameObject: " << m_Name << ", newParent is invalid\n";
        return;
    }


    if(newParent == nullptr)
    {
        SetLocalLocation(GetWorldLocation());
    }
    else
    {
        // if keep location, local position with adjust to newParent's world location
        // else, localPos, e.g. {100, 0, 0}, will also be {100, 0, 0} compared to newTarget
        // We could also have an option to reset LocalPos always to {0, 0, 0}, but why?
        if(bKeepLocation)
        {
            SetLocalLocation(GetWorldLocation() - newParent->GetWorldLocation());
        }

        SetLocationDirty();
    }


    if(m_Parent)
    {
        m_Parent->AttachChild(this, true);
    }

    m_Parent = newParent;

    if(m_Parent)
    {
        m_Parent->AttachChild(this);
    }
}

void GameObject::ForceDestroy()
{
    // Go DFS (Deep First Search)
    for(const auto pChild : m_Children)
    {
        if(pChild)
        {
            pChild->ForceDestroy();
        }
    }

    for(const auto pChild : m_Children)
    {
        if(pChild)
        {
            std::cout << "child should be dead?\n";
        }
    }

    m_Children.clear();
    m_MarkedForDeletion = true;


    // fuck it, let's crash
    delete this;
}

// this is for serialization, just as for all inputs
void GameObject::SetName(const std::string& newName)
{
    m_Name = newName;
}


#pragma region Tranform Functions


const glm::vec2& GameObject::GetWorldLocation() const
{
    return m_Transform->GetWorldLocation();
}

float GameObject::GetWorldRotation() const
{
    return m_Transform->GetWorldRotation();
}

const glm::vec2& GameObject::GetWorldScale() const
{
    return m_Transform->GetWorldScale();
}


const glm::vec2& GameObject::GetLocalLocation() const
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


void GameObject::SetWorldLocation(const glm::vec2& location) const
{
    m_Transform->SetWorldLocation(location);
}

void GameObject::SetWorldRotation(const float rotation) const
{
    m_Transform->SetWorldRotation(rotation);
}

void GameObject::SetWorldScale(const glm::vec2& scale) const
{
    m_Transform->SetWorldScale(scale);
}


void GameObject::SetLocalLocation(const glm::vec2& location) const
{
    m_Transform->SetLocalLocation(location);
}

void GameObject::SetLocalRotation(const float rotation) const
{
    m_Transform->SetLocalRotation(rotation);
}

void GameObject::SetLocalScale(const glm::vec2& scale) const
{
    m_Transform->SetLocalScale(scale);
}


void GameObject::AddLocation(const glm::vec2& addLocation) const
{
    m_Transform->AddLocation(addLocation);
}

void GameObject::AddRotation(const float addRotation) const
{
    m_Transform->AddRotation(addRotation);
}

void GameObject::AddScale(const glm::vec2& addScale) const
{
    m_Transform->AddScale(addScale);
}


void GameObject::SetLocationDirty() const
{
    m_Transform->SetLocationDirty();

    for(const GameObject* child : m_Children)
    {
        child->SetLocationDirty();
    }
}

void GameObject::SetRotationDirty() const
{
    m_Transform->SetRotationDirty();

    for(const GameObject* child : m_Children)
    {
        child->SetRotationDirty();
    }
}

void GameObject::SetScaleDirty() const
{
    m_Transform->SetScaleDirty();

    for(const GameObject* child : m_Children)
    {
        child->SetScaleDirty();
    }
}


#pragma endregion


