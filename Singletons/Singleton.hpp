#pragma once


namespace bae
{
    template<typename T>
    class Singleton
    {
    public:
        static T& GetInstance()
        {
            static T instance{};
            return instance;
        }

        virtual ~Singleton() = default;

        Singleton(const Singleton&)            = delete;
        Singleton(Singleton&&)                 = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&)      = delete;

    protected:
        Singleton() = default;
    };
}


