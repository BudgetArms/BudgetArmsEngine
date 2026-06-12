#include "EventQueue.hpp"

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
    if(eventListener)
    {
        m_ListenersToAdd.insert(eventListener);
    }
}

void EventQueue::RemoveListener(EventListener* eventListener)
{
    if(eventListener)
    {
        m_Listeners.erase(eventListener);
        m_ListenersToAdd.erase(eventListener);
        m_ListenersToRemove.erase(eventListener);
    }
}


void EventQueue::ProcessEvents()
{
    std::cout << "ProcessEvents" << '\n';
    for(auto& listenerToRemove : m_ListenersToRemove)
    {
        if(listenerToRemove)
        {
            std::cout << "Erased listener" << '\n';
            m_Listeners.erase(listenerToRemove);
        }
    }
    m_ListenersToRemove.clear();

    for(auto& listenerToAdd : m_ListenersToAdd)
    {
        std::cout << "Added listener" << '\n';
        if(listenerToAdd)
        {
            m_Listeners.insert(listenerToAdd);
        }
    }
    m_ListenersToAdd.clear();


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
    const auto listeners = m_Listeners;
    for(auto* listener : listeners)
    {
        if(m_Listeners.contains(listener))
        {
            listener->HandleEvent(eventHash);
        }
    }
}

