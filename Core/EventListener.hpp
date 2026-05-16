#pragma once

#include "Core/Event.hpp"
#include "Core/EventQueue.hpp"


namespace bae
{
    class EventListener
    {
    public:
        EventListener()
        {
            EventQueue::GetInstance().AddListener(this);
        }

        virtual ~EventListener() = default;


        virtual void HandleEvent(Event* event) = 0;
    };
}


