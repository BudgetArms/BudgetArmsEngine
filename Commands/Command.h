#pragma once


namespace bae
{
    class Command
    {
    public:
        explicit Command() = default;
        virtual ~Command() = default;

        Command(const Command& other)            = delete;
        Command(Command&& other)                 = delete;
        Command& operator=(const Command& other) = delete;
        Command& operator=(Command&& other)      = delete;


        virtual void Execute() = 0;
    };
}


