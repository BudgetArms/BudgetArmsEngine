#pragma once

#include <vector>

#include "Core/EventTypes.h"
#include "Core/GameObject.h"
#include "Core/Observer.h"


namespace bae
{
    class GameObject;

    class Subject
    {
    public:
        explicit Subject(GameObject& subjectGameObject);

        ~Subject() = default;

        void AddObserver(Observer* observer);
        void RemoveObserver(Observer* observer);

        [[nodiscard]] GameObject* GetGameObject() const;

    private:
        GameObject* m_GameObject{};
        std::vector<Observer*> m_Observers;

    protected:
        void NotifyObservers(EventType event);
    };
}


