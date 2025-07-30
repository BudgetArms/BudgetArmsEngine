#include "EventQueue.h"

#include <iostream>
#include <ranges>
#include <algorithm>
#include "Core/EventListener.h"


using namespace bae;


void EventQueue::AddEvent(const Event& event)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	if (IsFull())
	{
		std::cout << "EventQueue is full, can't add more\n";
		return;
	}

	m_Buffer[m_Head] = event;
	m_Head = (m_Head + 1) % m_Capacity;

	if (m_Head == m_Tail)
		m_bFull = true;

}

void EventQueue::AddListener(EventListener* eventListener)
{
	if (std::ranges::find(m_Listeners, eventListener) != m_Listeners.end())
	{
		std::cout << "Listener Already Added!!!\n";
		return;
	}

	m_Listeners.push_back(eventListener);
}

void EventQueue::RemoveListener(EventListener* eventListener)
{
	std::erase(m_Listeners, eventListener);
}


void EventQueue::ProcessEvents()
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	while (!IsEmpty())
	{
		if (m_Buffer[m_Tail].has_value())
		{
			ProcessEvent(m_Buffer[m_Tail].value());
			m_Buffer[m_Tail].reset();
		}

		m_Tail = (m_Tail + 1) % m_Capacity;
		m_bFull = false;
	}
}

void EventQueue::ProcessEvent(Event& event)
{
	for (auto listener : m_Listeners)
		if (listener)
			listener->HandleEvent(&event);

}


