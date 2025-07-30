#pragma once

#include <mutex>
#include <optional>
#include <vector>
#include <array>
#include "Singletons/Singleton.h"
#include "Core/Event.h"


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
		friend class Singleton<EventQueue>;
		EventQueue() = default;
		~EventQueue() = default;

		void ProcessEvent(Event& event);

		bool IsEmpty() const { return (!m_bFull && m_Head == m_Tail); }
		bool IsFull() const { return m_bFull; }


		static constexpr size_t m_Capacity{ 64 };
		std::array<std::optional<Event>, m_Capacity> m_Buffer{};

		size_t m_Head{};
		size_t m_Tail{};
		bool m_bFull{};

		std::mutex m_Mutex{};
		std::vector<EventListener*> m_Listeners{};


	};
}


