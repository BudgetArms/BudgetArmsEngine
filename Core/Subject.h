#pragma once

#include <vector>
#include "Core/EventTypes.h"
#include "Core/Observer.h"
#include "Core/GameObject.h"


namespace bae
{
	class Subject
	{
	public:
		explicit Subject(bae::GameObject& subjectGameObject) :
			m_GameObject{ &subjectGameObject }
		{
		}

		~Subject() = default;

		void AddObserver(Observer* observer)
		{
			m_Observers.push_back(observer);
		}
		void RemoveObserver(Observer* observer)
		{
			std::erase(m_Observers, observer);
		}

		GameObject* GetGameObject() const
		{
			return m_GameObject;
		}


	private:
		bae::GameObject* m_GameObject{};
		std::vector<Observer*> m_Observers;


	protected:
		void NotifyObservers(EventType event)
		{
			for (auto observer : m_Observers)
				observer->Notify(event, this);
		}


	};
}


