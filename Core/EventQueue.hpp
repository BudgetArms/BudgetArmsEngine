#pragma once

#include <any>
#include <set>

#include "RingBuffer.hpp"
#include "Singletons/Singleton.hpp"


namespace bae
{
    class EventListener;

    struct EventData
    {
        unsigned int Hash{};
        std::any Data{};
    };

    class EventQueue final : public Singleton<EventQueue>
    {
    public:
        void SendEvent(const EventData& eventData);

        void AddListener(EventListener* eventListener);
        void RemoveListener(EventListener* eventListener);

        void ProcessEvents();

    private:
        friend class Singleton;
        ~EventQueue() override = default;

        void ProcessEvent(const EventData& eventData) const;


        static constexpr size_t m_Capacity{ 64 };
        RingBuffer<EventData> m_Queue{ m_Capacity };

        std::set<EventListener*> m_Listeners{};

        std::set<EventListener*> m_ListenersToAdd{};
        std::set<EventListener*> m_ListenersToRemove{};
    };
}


