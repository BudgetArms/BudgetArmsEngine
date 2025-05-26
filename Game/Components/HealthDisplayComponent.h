#pragma once


#include "Core/Observer.h"
#include "Components/TextComponent.h"

class bae::GameObject;

namespace Game
{
    class HealthDisplayComponent final : public bae::TextComponent, public bae::Observer
    {
    public:
        HealthDisplayComponent(bae::GameObject& owner, std::shared_ptr<bae::Font> font);
        ~HealthDisplayComponent() = default;

        HealthDisplayComponent(const HealthDisplayComponent& other) = delete;
        HealthDisplayComponent(HealthDisplayComponent&& other) = delete;
        HealthDisplayComponent& operator=(const HealthDisplayComponent& other) = delete;
        HealthDisplayComponent& operator=(HealthDisplayComponent&& other) = delete;

        virtual void Notify(bae::Event event, bae::Subject* subject) override;


    };
}
