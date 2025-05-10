#include "TextComponent.h"

#include <stdexcept>
#include <SDL_ttf.h>

#include "Core/Renderer.h"
#include "Wrappers/Font.h"
#include "Wrappers/Texture2D.h"
#include "Managers/ResourceManager.h"

using namespace bae;


TextComponent::TextComponent(GameObject& owner, const std::string& text, std::shared_ptr<Font> font) :
    Component(owner),
    m_NeedsUpdate(true),
    m_Text(text),
    m_TextTexture(nullptr)
{
    if (!font)
        font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    m_Font = std::move(font);
}

void TextComponent::Update()
{
    if (m_NeedsUpdate)
    {
        const SDL_Color color = { 255, 255, 255, 255 }; // only white text is supported now

        const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
        if (!surf)
        {
            throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
        }

        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (!texture)
        {
            throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
        }

        SDL_FreeSurface(surf);
        m_TextTexture = std::make_shared<Texture2D>(texture);

        m_NeedsUpdate = false;
    }
}

void TextComponent::Render() const
{
    if (m_TextTexture)
    {
        const glm::vec3& pos = m_Owner->GetWorldLocation();
        const float& rotation = m_Owner->GetWorldRotation();
        const glm::vec2& scale = m_Owner->GetWorldScale();
        Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y, rotation, scale.x, scale.y);
    }
}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(const std::string& text)
{
    m_Text = text;
    m_NeedsUpdate = true;
}

void TextComponent::SetFont(std::shared_ptr<Font> font)
{
    m_Font = font;
    m_NeedsUpdate = true;
}
