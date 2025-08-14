#include "FpsCounterComponent.h"
#include <iostream>

#include "Wrappers/Font.h"
#include "Components/TextComponent.h"
#include "Singletons/GameTime.h"
#include "Wrappers/Texture2D.h"
#include "Core/Utils.h"


using namespace bae;

FpsTextComponent::FpsTextComponent(GameObject& owner, std::shared_ptr<Font> font, SDL_Color color) :
	TextComponent(owner, "FPS: xxx", font)
{
	SetColor(color);
}


void FpsTextComponent::Update()
{
	TextComponent::Update(); // update text if needed

	m_AccumulatedTime += GameTime::GetInstance().GetDeltaTime();
	if (m_AccumulatedTime > 1 / m_NrUpdatesPerSecond)
	{
		m_AccumulatedTime = 0;
		const int fps = static_cast<int>(GameTime::GetInstance().GetFPS());
		SetText("FPS: " + std::to_string(fps));
	}

}

void FpsTextComponent::Render() const
{
	const glm::vec3& pos = m_Owner->GetWorldLocation() - glm::vec3{ 2.f, 2.f, 0.f };
	const float& rotation = m_Owner->GetWorldRotation();
	const glm::vec2& scale = m_Owner->GetWorldScale();

	const int width{ 75 };
	const int height{ 20 };

	const int newWidthHalf = static_cast<int>(width * std::abs(scale.x) / 2.f);
	const int newHeightHalf = static_cast<int>(height * std::abs(scale.y) / 2.f);

	const int centerX = static_cast<int>(pos.x + newWidthHalf);
	const int centerY = static_cast<int>(pos.y + newHeightHalf);


	float finalAngle = rotation;
	if (scale.x < 0.f)
		finalAngle = 180.f - finalAngle;
	if (scale.x < 0.f)
		finalAngle = -finalAngle;


	float angleRad = finalAngle * static_cast<float>(M_PI) / 180.f;
	float cosA = cosf(angleRad);
	float sinA = sinf(angleRad);

	std::vector<glm::vec2> points =
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
