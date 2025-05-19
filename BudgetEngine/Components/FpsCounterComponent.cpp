#include "FpsCounterComponent.h"
#include <iostream>

#include "Wrappers/Font.h"
#include "Components/TextComponent.h"
#include "Singletons/GameTime.h"
#include "Wrappers/Texture2D.h"


using namespace bae;

FpsTextComponent::FpsTextComponent(GameObject& owner, std::shared_ptr<Font> font) :
    TextComponent(owner, "FPS: xxx", font)
{
}


void FpsTextComponent::Update()
{
    m_AccumulatedTime += GameTime::GetInstance().GetDeltaTime();
    if (m_AccumulatedTime > 1 / m_NrUpdatesPerSecond)
    {
        m_AccumulatedTime = 0;
        const int fps = static_cast<int>(GameTime::GetInstance().GetFPS());
        SetText("FPS: " + std::to_string(fps));
        TextComponent::Update();
    }

}

