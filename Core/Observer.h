#pragma once
#include <cstdint>

#include "Core/GameObject.h"
#include "Core/EventTypes.h"


namespace bae
{
	class Subject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(EventType event, Subject* subject) = 0;


	};
}


