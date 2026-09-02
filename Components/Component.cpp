#include "Component.hpp"

#include "Core/HelperFunctions.hpp"


using namespace bae;


Component::Component(GameObject& owner) :
    m_Owner{ &owner }
{
}

Component::~Component()
{
    // if it gets deleted with being marked for deletion && it's the project closing; assert
    if(!m_bIsMarkedForDeletion)
    {
        const std::string errorMessage = std::string(FUNCTION_NAME) +
                " Failed! The Component was not marked for destruction\n";

        std::cout << errorMessage;
        assert(false && errorMessage.c_str());
    }
}

void Component::Destroy()
{
    m_bIsMarkedForDeletion = true;
}

bool Component::IsValid(const Component* component)
{
    if(component && !component->IsMarkedDeletion())
    {
        return true;
    }

    return false;
}

GameObject* Component::GetOwner()
{
    return m_Owner;
}

bool Component::IsMarkedDeletion() const
{
    return m_bIsMarkedForDeletion;
}
