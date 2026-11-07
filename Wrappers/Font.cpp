#include "Font.hpp"

// used for std::terminate
#include <exception>

#include <SDL3_ttf/SDL_ttf.h>

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"


bae::Font::Font(const std::string& fullPath, const float size)
{
    m_Font = TTF_OpenFont(fullPath.c_str(), size);
    if(m_Font == nullptr)
    {
        Logger::LogFatal("Failed to load font, Error: " + std::string(SDL_GetError()));
        std::terminate();
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
