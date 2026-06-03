#pragma once

#include <set>

#include "RingBuffer.hpp"
#include "Singletons/Singleton.hpp"


namespace bae
{
    class EventListener;

    class EventQueue final : public Singleton<EventQueue>
    {
    public:
        void SendEvent(unsigned int eventHash);

        void AddListener(EventListener* eventListener);
        void RemoveListener(EventListener* eventListener);

        void ProcessEvents();

    private:
        friend class Singleton;
        ~EventQueue() override = default;

        void ProcessEvent(unsigned int eventHash) const;


        static constexpr size_t m_Capacity{ 64 };
        RingBuffer<unsigned int> m_Queue{ m_Capacity };

        std::set<EventListener*> m_Listeners{};

        std::set<EventListener*> m_ListenersToAdd{};
        std::set<EventListener*> m_ListenersToRemove{};
    };
}


