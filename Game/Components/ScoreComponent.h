#pragma once


#include "Core/Subject.h"
#include "Components/Component.h"


namespace Game
{
    class ScoreComponent final : public bae::Component, public bae::Subject
    {
    public:
        ScoreComponent(bae::GameObject& owner);
        ~ScoreComponent() = default;

        ScoreComponent(const ScoreComponent& other) = delete;
        ScoreComponent(ScoreComponent&& other) = delete;
        ScoreComponent& operator=(const ScoreComponent& other) = delete;
        ScoreComponent& operator=(ScoreComponent&& other) = delete;


        int GetScore() const { return m_Score; };
        void AddScore(int score);
        void SubtractScore(int score);


    private:
        int m_Score{ 10 };


    };
}