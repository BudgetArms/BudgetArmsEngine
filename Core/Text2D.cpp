#include "Text2D.hpp"

#include <string>
#include <SDL3_ttf/SDL_ttf.h>

#include "HelperFunctions.hpp"
#include "Core/Renderer.hpp"
#include "Managers/ResourceManager.hpp"


using namespace bae;


Text2D::Text2D(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color) :
    m_bIsCenteredAtPosition{ false },
    m_Position{},
    m_Rotation{},
    m_Scale{ 1.f, 1.f },
    m_bNeedsUpdate{ true },
    m_Text{ text },
    m_Color{ color }
{
    if(!font)
    {
        font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 36);
    }

    m_uFont = std::move(font);
}


void Text2D::Update()
{
    if(m_bNeedsUpdate)
    {
        const auto surface = TTF_RenderText_Blended(m_uFont->GetFont(), m_Text.c_str(), m_Text.length(), m_Color);
        if(!surface)
        {
            throw std::runtime_error(
                FUNCTION_NAME + std::string(" Failed to render text/font, Error: ") + SDL_GetError());
        }

        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
        if(!texture)
        {
            throw std::runtime_error(
                FUNCTION_NAME + std::string(" Failed to create texture from surface, Error: ") + SDL_GetError());
        }

        SDL_DestroySurface(surface);
        m_uTextTexture = std::make_unique<Texture2D>(texture);

        m_bNeedsUpdate = false;
    }
}

void Text2D::Render() const
{
    if(m_uTextTexture)
    {
        Renderer::GetInstance().RenderTexture(*m_uTextTexture, m_bIsCenteredAtPosition,
                                              m_Position, m_Rotation, m_Scale);
    }
}


std::string Text2D::GetText()
{
    if(m_bNeedsUpdate)
    {
        Update();
    }

    return m_Text;
}

void Text2D::SetText(const std::string& text)
{
    m_Text         = text;
    m_bNeedsUpdate = true;
}


SDL_Color Text2D::GetColor()
{
    if(m_bNeedsUpdate)
    {
        Update();
    }

    return m_Color;
}

void Text2D::SetColor(const SDL_Color& color)
{
    m_Color        = color;
    m_bNeedsUpdate = true;
}


void Text2D::SetFont(std::shared_ptr<Font> font)
{
    m_uFont        = std::move(font);
    m_bNeedsUpdate = true;
}


