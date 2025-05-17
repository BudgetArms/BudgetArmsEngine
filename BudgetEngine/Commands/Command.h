#pragma once
#include "Core/GameObject.h"


namespace bae
{
    class Command
    {
    public:
        explicit Command() {};
        virtual ~Command() = default;

        Command(const Command& other) = delete;
        Command(Command&& other) = delete;
        Command& operator=(const Command& other) = delete;
        Command& operator=(Command&& other) = delete;

        virtual void Execute() = 0;


    private:


    protected:


    };


}


