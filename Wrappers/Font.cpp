#include <stdexcept>

#include <SDL3_ttf/SDL_ttf.h>

#include "Font.h"

#include "Core/HelperFunctions.h"


bae::Font::Font(const std::string& fullPath, const float size) : m_Font(nullptr)
{
    m_Font = TTF_OpenFont(fullPath.c_str(), size);
    if(m_Font == nullptr)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to load font, Error: ") + SDL_GetError());
    }
}

bae::Font::~Font()
{
    TTF_CloseFont(m_Font);
}


TTF_Font* bae::Font::GetFont() const
{
    return m_Font;
}


