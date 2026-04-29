#pragma once

#include "Components/TextComponent.h"
#include "Core/Utils.h"


namespace bae
{
    class GameObject;
    class Font;

    class FpsTextComponent final : public TextComponent
    {
    public:
        explicit FpsTextComponent(GameObject& owner,
                                  std::shared_ptr<Font> font = nullptr,
                                  const Utils::Color& color  = Utils::Color::White);
        ~FpsTextComponent() override = default;

        FpsTextComponent(const FpsTextComponent& other)            = delete;
        FpsTextComponent(FpsTextComponent&& other)                 = delete;
        FpsTextComponent& operator=(const FpsTextComponent& other) = delete;
        FpsTextComponent& operator=(FpsTextComponent&& other)      = delete;

        void Update() override;
        void Render() const override;

    private:
        float m_AccumulatedTime{};
        const float m_NrUpdatesPerSecond{ 1.f };
    };
}

