#include "Text2D.h"

#include <SDL_ttf.h>

#include "Managers/ResourceManager.h"
#include "Core/Renderer.h"


using namespace bae;


Text2D::Text2D(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color) :
	m_bIsCenteredAtPosition{ false },
	m_Position{},
	m_Rotation{},
	m_Scale{ 1.f, 1.f },
	m_uFont{},
	m_uTextTexture{},
	m_bNeedsUpdate{ true },
	m_Text{ text },
	m_Color{ color }
{
	if (!font)
		font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 36);

	m_uFont = std::move(font);
}


void Text2D::Update()
{
	if (m_bNeedsUpdate)
	{

		const auto surf = TTF_RenderText_Blended(m_uFont->GetFont(), m_Text.c_str(), m_Color);
		if (!surf)
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (!texture)
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		m_uTextTexture = std::make_unique<Texture2D>(texture);

		m_bNeedsUpdate = false;
	}

}

void Text2D::Render() const
{
	if (m_uTextTexture)
		Renderer::GetInstance().RenderTexture(*m_uTextTexture, m_bIsCenteredAtPosition, m_Position, m_Rotation, m_Scale);

}



std::string Text2D::GetText()
{
	if (m_bNeedsUpdate)
		Update();

	return m_Text;
}

void Text2D::SetText(const std::string& text)
{
	m_Text = text;
	m_bNeedsUpdate = true;
}


SDL_Color Text2D::GetColor()
{
	if (m_bNeedsUpdate)
		Update();

	return m_Color;
}

void Text2D::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_bNeedsUpdate = true;
}


void Text2D::SetFont(std::shared_ptr<Font> font)
{
	m_uFont = font;
	m_bNeedsUpdate = true;
}


