#pragma once

#include "Core/Observer.h"
#include "Core/Subject.h"


namespace Game
{

    class Achievement : public bae::Observer
    {
    public:
        enum class Type
        {
            FirstBlood,
            FirstWin,
            FirstDeath,
            ReachedLevel50,
        };

        void Notify(bae::Event event, bae::Subject* subject) override
        {
            switch (event)
            {
            case bae::Event::PLAYER_DIED:
                //case GameEvent::ActorDied:
                //if (CheckIfNobodyElseDiedYet(actor))
                if (subject)
                {
                    Unlock(Achievement::Type::FirstBlood);
                }
                break;
            case bae::Event::PLAYER_LEVEL_UP:
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
            case Achievement::Type::FirstBlood:
                m_bFirstBlood = true;
                break;
            case Achievement::Type::FirstWin:
                m_bFirstWin = true;
                break;
            case Achievement::Type::FirstDeath:
                m_bFirstDeath = true;
                break;
            case Achievement::Type::ReachedLevel50:
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

}

