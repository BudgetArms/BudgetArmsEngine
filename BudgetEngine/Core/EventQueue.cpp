#include "EventQueue.h"

#include <iostream>
#include <ranges>
#include <algorithm>
#include "Core/EventListener.h"


using namespace bae;

void EventQueue::AddListener(EventListener* eventListener)
{
    if (std::ranges::find(m_Listeners, eventListener) == m_Listeners.end())
    {
        std::cout << "Listener Already Added!!!\n";
        return;
    }

    m_Listeners.push_back(eventListener);
}













