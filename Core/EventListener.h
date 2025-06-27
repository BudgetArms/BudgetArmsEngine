#pragma once

#include "Core/EventQueue.h"
#include "Core/Event.h"


namespace bae
{
    class EventListener
    {
    public:
        EventListener()
        {
            EventQueue::GetInstance().AddListener(this);
        }

        ~EventListener() = default;

        virtual void HandleEvent(Event* event) = 0;



    private:


    };
}

