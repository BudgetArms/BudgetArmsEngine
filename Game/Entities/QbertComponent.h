#pragma once

#include <string>
#include <iostream>

#include "Components/Component.h"


namespace Game
{
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


    private:
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

        enum class QBertStates
        {
            Idle,
            Jumping,
            Floating,
            Dying,
        };



    };

}


