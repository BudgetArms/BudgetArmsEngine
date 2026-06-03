#include "EventQueue.hpp"

#include <algorithm>
#include <iostream>

#include "Core/EventListener.hpp"
#include "Core/HelperFunctions.hpp"


using namespace bae;


void EventQueue::SendEvent(const unsigned int eventHash)
{
    if(m_Queue.IsFull())
    {
        std::cout << FUNCTION_NAME << " Failed! EventQueue is full, can't add more" << '\n';
        return;
    }

    const bool success = m_Queue.Push(eventHash);
    if(!success)
    {
        std::cout << FUNCTION_NAME << " Failed! EventQueue is full, after trying push" << '\n';
    }
}

void EventQueue::AddListener(EventListener* eventListener)
{
    m_Listeners.insert(eventListener);
}

void EventQueue::RemoveListener(EventListener* eventListener)
{
    m_ListenersToAdd.insert(eventListener);
}


void EventQueue::ProcessEvents()
{
    for(auto& listenerToAdd : m_ListenersToAdd)
    {
        m_Listeners.insert(listenerToAdd);
    }
    m_ListenersToAdd.clear();

    for(auto& listenerToRemove : m_ListenersToRemove)
    {
        m_Listeners.erase(listenerToRemove);
    }
    m_ListenersToRemove.clear();

    while(!m_Queue.IsEmpty())
    {
        unsigned int eventHash{ 0 };
        m_Queue.Pop(eventHash);

        // if Event Valid
        if(eventHash != 0)
        {
            ProcessEvent(eventHash);
        }
    }
}

void EventQueue::ProcessEvent(const unsigned int eventHash) const
{
    for(EventListener* eventListener : m_Listeners)
    {
        if(eventListener)
        {
            eventListener->HandleEvent(eventHash);
        }
    }
}

