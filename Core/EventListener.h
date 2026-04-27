#pragma once

#include "Core/Event.h"
#include "Core/EventQueue.h"


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


