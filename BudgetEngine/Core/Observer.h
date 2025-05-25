#pragma once
#include <cstdint>

#include "Core/GameObject.h"
#include "Core/Events.h"



namespace bae
{
    class Subject;

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(Event event, Subject* subject) = 0;

    };


}


