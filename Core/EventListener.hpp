#pragma once

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

        EventListener(const EventListener&)            = delete;
        EventListener(EventListener&&)                 = delete;
        EventListener& operator=(const EventListener&) = delete;
        EventListener& operator=(EventListener&&)      = delete;

        virtual void HandleEvent(Event* event) = 0;
    };
}


