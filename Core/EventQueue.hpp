#pragma once

#include <array>
#include <mutex>
#include <optional>
#include <vector>

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

        std::vector<EventListener*> m_Listeners{};
    };
}


