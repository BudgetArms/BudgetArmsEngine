#include "FpsCounterComponent.h"

#include <numbers>

#include "Components/TextComponent.h"
#include "Core/Utils.h"
#include "Singletons/GameTime.h"
#include "Wrappers/Font.h"
#include "Wrappers/Texture2D.h"


using namespace bae;

FpsTextComponent::FpsTextComponent(GameObject& owner, std::shared_ptr<Font> font, SDL_Color color) :
    TextComponent(owner, "FPS: xxx", std::move(font))
{
    SetColor(color);
}


void FpsTextComponent::Update()
{
    TextComponent::Update(); // update text if needed

    m_AccumulatedTime += GameTime::GetInstance().GetDeltaTime();
    if(m_AccumulatedTime > 1 / m_NrUpdatesPerSecond)
    {
        m_AccumulatedTime = 0;
        const int fps     = static_cast<int>(GameTime::GetInstance().GetFPS());
        SetText("FPS: " + std::to_string(fps));
    }
}

void FpsTextComponent::Render() const
{
    const glm::vec2& pos   = m_Owner->GetWorldLocation() - glm::vec2{ 2.f, 2.f };
    const float& rotation  = m_Owner->GetWorldRotation();
    const glm::vec2& scale = m_Owner->GetWorldScale();

    constexpr int width{ 75 };
    constexpr int height{ 20 };

    const auto newWidthHalf  = static_cast<float>(static_cast<int>(width * std::abs(scale.x) / 2.f));
    const auto newHeightHalf = static_cast<float>(static_cast<int>(height * std::abs(scale.y) / 2.f));

    const auto centerX = static_cast<float>(static_cast<int>(pos.x)) + newWidthHalf;
    const auto centerY = static_cast<float>(static_cast<int>(pos.y)) + newHeightHalf;


    float finalAngle = rotation;
    if(scale.x < 0.f)
    {
        finalAngle = 180.f - finalAngle;
    }
    if(scale.x < 0.f)
    {
        finalAngle = -finalAngle;
    }


    const float angleRad = finalAngle * static_cast<float>(std::numbers::pi) / 180.f;
    const float cosA     = cosf(angleRad);
    const float sinA     = sinf(angleRad);

    const std::vector<glm::vec2> points =
    {
        {
            centerX - newWidthHalf * cosA + newHeightHalf * sinA,
            centerY - newWidthHalf * sinA - newHeightHalf * cosA
        },
        {
            centerX + newWidthHalf * cosA + newHeightHalf * sinA,
            centerY + newWidthHalf * sinA - newHeightHalf * cosA
        },
        {
            centerX + newWidthHalf * cosA - newHeightHalf * sinA,
            centerY + newWidthHalf * sinA + newHeightHalf * cosA
        },
        {
            centerX - newWidthHalf * cosA - newHeightHalf * sinA,
            centerY - newWidthHalf * sinA + newHeightHalf * cosA
        }
    };


    Utils::FillPolygon(points, Utils::Color::DarkGray);

    TextComponent::Render();
}
