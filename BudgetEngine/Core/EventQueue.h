#pragma once

#include <vector>
#include "Singletons/Singleton.h"
//#include "Core/EventListener.h"


namespace bae
{
    class EventListener;

    class EventQueue final : public Singleton<EventQueue>
    {
    public:

        void AddListener(EventListener* eventListener);



    private:
        friend class Singleton<EventQueue>;
        EventQueue() = default;
        ~EventQueue() = default;


        std::vector<EventListener*> m_Listeners;



    };

}

