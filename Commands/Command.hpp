#pragma once


namespace bae
{
    class Command
    {
    public:
        explicit Command() = default;
        virtual ~Command() = default;

        Command(const Command&)            = delete;
        Command(Command&&)                 = delete;
        Command& operator=(const Command&) = delete;
        Command& operator=(Command&&)      = delete;


        virtual void Execute() = 0;
        [[nodiscard]] virtual bool IsValid() const { return true; }
    };
}


