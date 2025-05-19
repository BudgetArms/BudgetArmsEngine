#pragma once
#include <cstdint>

#include "Core/GameObject.h"


namespace bae
{
    class Subject;

    struct EventArg {};
    enum class EventId
    {
        LEVEL_STARTED,
        BOMB_EXPLODED,
        PLAYER_DIED,
        PLAYER_LEVEL_UP,
    };

    // why not enum class???
    struct Event
    {
        const EventId ID;
        static const uint8_t MAX_ARGS = 8;
        uint8_t nbArgs;
        EventArg args[MAX_ARGS];
        explicit Event(EventId id) : ID{ id } {}
    };

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(Event event, Subject* subject) = 0;

    };

    class Achievement : public Observer
    {
    public:
        enum class Type
        {
            FirstBlood,
            FirstWin,
            FirstDeath,
            ReachedLevel50,
        };

        void Notify(Event event, Subject* subject) override
        {
            switch (event.ID)
            {
            case EventId::PLAYER_DIED:
                //case GameEvent::ActorDied:
                //if (CheckIfNobodyElseDiedYet(actor))
                if (subject)
                {
                    Unlock(Achievement::Type::FirstBlood);
                }
                break;
            case EventId::PLAYER_LEVEL_UP:
                if (subject)
                {
                    // check if hit level 50
                    if (true)
                        Unlock(Achievement::Type::ReachedLevel50);
                }
                break;


                //case GameEvent::ActorFell:
                    // etc...
            }
        }


    private:
        void Unlock(Achievement::Type achievement)
        {
            // code to unlock an achievement...
            switch (achievement)
            {
            case bae::Achievement::Type::FirstBlood:
                m_bFirstBlood = true;
                break;
            case bae::Achievement::Type::FirstWin:
                m_bFirstWin = true;
                break;
            case bae::Achievement::Type::FirstDeath:
                m_bFirstDeath = true;
                break;
            case bae::Achievement::Type::ReachedLevel50:
                m_bReachedLevel50 = true;
                break;
            default:
                break;
            }
        }

        bool m_bFirstBlood{ false };
        bool m_bFirstWin{ false };
        bool m_bFirstDeath{ false };
        bool m_bReachedLevel50{ false };
        bool m_bReachedLevel100{ false };
        bool m_bReachedLevel150{ false };
        bool m_bReachedLevel200{ false };
        bool m_bReachedLevel250{ false };


    };

    class Subject
    {
    public:
        void AddObserver(Observer* observer)
        {
            // code to add an observer
            m_observers.push_back(observer);
        }
        void RemoveObserver(Observer* observer)
        {
            // code to remove an observer
            std::erase(m_observers, observer);
        }


    private:
        std::vector<Observer*> m_observers;

    protected:
        void NotifyObservers(Event event)
        {
            for (auto observer : m_observers)
                observer->Notify(event, this);
        }


    };


}


