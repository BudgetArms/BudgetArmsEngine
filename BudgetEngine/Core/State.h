#pragma once


namespace bae
{
    class State
    {
    public:
        State() = default;
        virtual ~State() = default;


        virtual void Enter() = 0;
        virtual void Exit() = 0;

        virtual void Update() = 0;


    };
}


