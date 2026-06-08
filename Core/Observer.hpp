#pragma once

#include <any>


namespace bae
{
    class Subject;

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(unsigned int eventHash, Subject* subject, const std::any& eventData) = 0;
    };
}

