#pragma once

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

        ~EventListener() = default;

        //void HandleEvent(Event*) = 0;



    private:


    };
}

