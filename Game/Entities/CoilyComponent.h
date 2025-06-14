#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Components/Component.h"

#include "Base/CoilyStates.h"


namespace Game
{
    enum class CoilySprites
    {
        UpRightCrouched,
        UpRightStand,
        UpLeftCrouched,
        UpLeftStand,
        DownRightCrouched,
        DownRightStand,
        DownLeftCrouched,
        DownLeftStand
    };


    class CoilyComponent final : public bae::Component
    {
    public:
        explicit CoilyComponent(bae::GameObject& owner);
        ~CoilyComponent() = default;

        CoilyComponent(const CoilyComponent& other) = delete;
        CoilyComponent(CoilyComponent&& other) = delete;
        CoilyComponent& operator=(const CoilyComponent& other) = delete;
        CoilyComponent& operator=(CoilyComponent&& other) = delete;


        virtual void Update() override;
        virtual void Render() const override;

        void HandleInput(const glm::vec2& dir);


    private:
        Game::States::CoilyStateComponent* m_StateComponent;


    };

}


