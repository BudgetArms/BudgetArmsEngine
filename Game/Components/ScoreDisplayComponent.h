#pragma once


#include "Core/Observer.h"
#include "Components/TextComponent.h"


namespace Game
{
    class ScoreDisplayComponent final : public bae::TextComponent, public bae::Observer
    {
    public:
        ScoreDisplayComponent(bae::GameObject& owner, std::shared_ptr<bae::Font> font);
        ~ScoreDisplayComponent() = default;

        ScoreDisplayComponent(const ScoreDisplayComponent& other) = delete;
        ScoreDisplayComponent(ScoreDisplayComponent&& other) = delete;
        ScoreDisplayComponent& operator=(const ScoreDisplayComponent& other) = delete;
        ScoreDisplayComponent& operator=(ScoreDisplayComponent&& other) = delete;

        virtual void Notify(bae::EventType event, bae::Subject* subject) override;


    };
}