#pragma once


namespace bae
{
    class Subject;

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(unsigned int eventHash, Subject* subject) = 0;
    };
}

