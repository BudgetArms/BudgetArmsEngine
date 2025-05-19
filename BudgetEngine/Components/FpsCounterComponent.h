#pragma once

#include "Components/TextComponent.h"


namespace bae
{
    class GameObject;
    class Font;
    class FpsTextComponent final : public TextComponent
    {
    public:
        explicit FpsTextComponent(GameObject& owner, std::shared_ptr<Font> font = nullptr);
        virtual ~FpsTextComponent() = default;

        FpsTextComponent(const FpsTextComponent& other) = delete;
        FpsTextComponent(FpsTextComponent&& other) = delete;
        FpsTextComponent& operator=(const FpsTextComponent& other) = delete;
        FpsTextComponent& operator=(FpsTextComponent&& other) = delete;

        virtual void Update() override;


    private:
        bool m_bRotating{ true };
        float m_AccumulatedTime{};
        const float m_NrUpdatesPerSecond{ 1.f };


    protected:


    };

}


