#pragma once

#include <string>
#include <iostream>

#include "Components/Component.h"


namespace Game
{
    class GameObject;

    class QbertComponent : public bae::Component
    {
    public:
        explicit QbertComponent(bae::GameObject& owner);
        ~QbertComponent() = default;

        QbertComponent(const QbertComponent& other) = delete;
        QbertComponent(QbertComponent&& other) = delete;
        QbertComponent& operator=(const QbertComponent& other) = delete;
        QbertComponent& operator=(QbertComponent&& other) = delete;


    private:
        enum class QBertState
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



    protected:


    };

}


