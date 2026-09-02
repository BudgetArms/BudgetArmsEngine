#include "GameObject.hpp"

#include <cassert>
#include <iostream>
#include <string>

#include "Components/Component.hpp"
#include "Components/TransformComponent.hpp"
#include "Core/HelperFunctions.hpp"
#include "Core/Renderer.hpp"


using namespace bae;

bool GameObject::m_bDebugCreation    = false;
bool GameObject::m_bDebugDestruction = false;


GameObject::GameObject(const std::string& name)
{
    SetName(name);

    if(m_bDebugDestruction)
    {
        std::cout << FUNCTION_NAME << " Name: " << m_Name << '\n';
    }

    AddComponent<TransformComponent>(*this);
    m_Transform = GetComponent<TransformComponent>();
}

GameObject::~GameObject()
{
    // my design philosophy:
    // Shared pointers: reset in LateUpdate if Marked For Deletion
    // Unique pointers: release in LateUpdate if Marked For Deletion

    // No Object/class should destroy itself.

    if(m_bDebugDestruction)
    {
        std::cout << FUNCTION_NAME << " Name: " << m_Name << '\n';
    }

    if(IsValid(m_Parent))
    {
        m_Parent->DetachChild(*this);
    }

    // if it gets deleted with being marked for deletion && it's the project closing; assert
    if(!m_MarkedForDeletion)
    {
        const std::string errorMessage = std::string(FUNCTION_NAME)
                + " Failed! The GameObject \"" + m_Name + "\" was not marked for destruction";

        std::cout << errorMessage << '\n';
        assert(false && errorMessage.c_str());
    }

    for(auto& component : m_Components)
    {
        component->Destroy();
        component = nullptr;
    }
}

void GameObject::Update() const
{
    for(const GameObject* pChild : m_Children)
    {
        if(IsValid(pChild))
        {
            pChild->Update();
        }
    }

    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(Component::IsValid(component.get()))
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
        if(component && !component->IsMarkedDeletion())
        {
            component->FixedUpdate();
        }
    }
}

void GameObject::LateUpdate()
{
    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(Component::IsValid(component.get()))
        {
            component->LateUpdate();
        }
    }

    std::erase_if(m_Components, [](const std::unique_ptr<Component>& component)
    {
        return component->IsMarkedDeletion();
    });
}

void GameObject::Render() const
{
    for(const std::unique_ptr<Component>& component : m_Components)
    {
        if(Component::IsValid(component.get()))
        {
            component->Render();
        }
    }
}

void GameObject::RenderGUI() const
{
    for(const auto& component : m_Components)
    {
        if(Component::IsValid(component.get()))
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
        if(pChild && !pChild->IsMarkedForDeletion())
        {
            pChild->Destroy();
        }
    }
}


void GameObject::AttachChild(GameObject& child, const bool bFreezeLocation, const bool bFreezeRotation,
                             const bool bFreezeScale)
{
    // if invalid
    if(this == &child || m_Parent == &child)
    {
        std::cout << "GameObject: " << m_Name << ", AttachChild: newChild is invalid\n";
        return;
    }

    if(IsChild(&child))
    {
        std::cout << "GameObject: " << m_Name << ", AttachChild: newChild (" << child.m_Name
                << ") is Already a Child\n";
        return;
    }

    if(bFreezeLocation)
    {
        child.SetLocalLocation(child.GetWorldLocation() - GetWorldLocation());
    }

    if(bFreezeRotation)
    {
        child.SetLocalRotation(child.GetWorldRotation() - GetWorldRotation());
    }

    if(bFreezeScale)
    {
        child.SetLocalScale(child.GetWorldScale() / GetWorldScale());
    }

    child.SetLocationDirty();
    child.SetRotationDirty();
    child.SetScaleDirty();

    // this removes the child from the parent's children list
    if(child.m_Parent)
    {
        std::erase(child.m_Parent->m_Children, &child);
    }

    // Make myself parent from newChild (get adopted) :D
    child.m_Parent = this;

    // YOU ARE NOW MY CHILD!!!
    m_Children.emplace_back(&child);
}

void GameObject::DetachChild(GameObject& child, const bool bUpdateChildrenOfChildLocations)
{
    // if invalid
    if(this == &child || m_Parent == &child)
    {
        std::cout << "GameObject: " << m_Name << ", DetachChild: child is invalid\n";
        return;
    }

    // if invalid
    if(!IsChild(&child))
    {
        std::cout << "GameObject: " << m_Name << ", DetachChild: child ("
                << child.m_Name << ") is Not a Child\n";
        return;
    }


    if(bUpdateChildrenOfChildLocations)
    {
        child.SetLocalLocation(GetWorldLocation());
    }

    child.SetLocationDirty();
    child.SetRotationDirty();
    child.SetScaleDirty();


    // remove deleteChild from my child list
    std::erase(m_Children, &child);

    child.m_Parent = nullptr;
}

bool GameObject::IsChild(const GameObject* child) const
{
    if(m_Children.empty())
    {
        return false;
    }

    return std::ranges::find(m_Children, child) != m_Children.end();
}

bool GameObject::IsValid(const GameObject* object)
{
    if(object && !object->IsMarkedForDeletion())
    {
        return true;
    }

    return false;
}

GameObject* GameObject::GetParent() const
{
    return m_Parent;
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
