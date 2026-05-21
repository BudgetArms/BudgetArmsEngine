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
    if(std::ranges::find(m_Listeners, eventListener) != m_Listeners.end())
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
    for(EventListener* listener : m_Listeners)
    {
        if(listener)
        {
            listener->HandleEvent(eventHash);
        }
    }
}

