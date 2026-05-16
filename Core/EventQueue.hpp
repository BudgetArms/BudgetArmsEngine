#pragma once

#include <array>
#include <mutex>
#include <optional>
#include <vector>

#include "Core/Event.hpp"
#include "Singletons/Singleton.hpp"


namespace bae
{
    class EventListener;

    class EventQueue final : public Singleton<EventQueue>
    {
    public:
        void AddEvent(const Event& event);

        void AddListener(EventListener* eventListener);
        void RemoveListener(EventListener* eventListener);

        void ProcessEvents();

    private:
        friend class Singleton;
        EventQueue()           = default;
        ~EventQueue() override = default;

        void ProcessEvent(Event& event) const;

        [[nodiscard]] bool IsEmpty() const { return !m_bFull && m_Head == m_Tail; }
        [[nodiscard]] bool IsFull() const { return m_bFull; }


        static constexpr size_t m_Capacity{ 64 };
        std::array<std::optional<Event>, m_Capacity> m_Buffer{};

        size_t m_Head{};
        size_t m_Tail{};
        bool m_bFull{};

        std::mutex m_Mutex{};
        std::vector<EventListener*> m_Listeners{};
    };
}


