#pragma once

#include <any>

#include "EventQueue.hpp"


namespace bae
{
    class Subject;

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(const EventData& eventData, Subject* subject) = 0;
    };
}

