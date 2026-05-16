#include "Subject.hpp"

#include "GameObject.hpp"


using namespace bae;


Subject::Subject(GameObject& subjectGameObject) :
    m_GameObject{ &subjectGameObject }
{
}

void Subject::AddObserver(Observer* observer)
{
    m_Observers.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
    std::erase(m_Observers, observer);
}

GameObject* Subject::GetGameObject() const
{
    return m_GameObject;
}

void Subject::NotifyObservers(const EventType event)
{
    for(Observer* observer : m_Observers)
    {
        observer->Notify(event, this);
    }
}

