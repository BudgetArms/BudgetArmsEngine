#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Components/Component.h"

#include "Base/QbertStates.h"


namespace Game
{
    enum class QBertSprites
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

    class GameObject;

    class QbertComponent final : public bae::Component
    {
    public:
        explicit QbertComponent(bae::GameObject& owner);
        ~QbertComponent() = default;

        QbertComponent(const QbertComponent& other) = delete;
        QbertComponent(QbertComponent&& other) = delete;
        QbertComponent& operator=(const QbertComponent& other) = delete;
        QbertComponent& operator=(QbertComponent&& other) = delete;


        virtual void Update() override;
        virtual void Render() const override;


    private:
        std::unique_ptr<Game::States::QbertState> m_State;


    };

}


