#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include "Components/Component.h"
#include "Components/SpriteComponent.h"
#include "Wrappers/Texture2D.h"

#include "Base/QbertStates.h"

//namespace bae
//{
    //class Texture2D;
//};

namespace Game
{
    enum class QbertSprites
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

        void HandleInput(const glm::vec2& dir);


    private:
        Game::States::QbertStateComponent* m_StateComponent;

        // because the component system I have now is incomplete for the job required
        std::vector<std::unique_ptr<bae::Component>> m_Components;

    };

}


