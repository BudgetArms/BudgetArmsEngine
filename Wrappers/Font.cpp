#include <stdexcept>

#include <SDL3_ttf/SDL_ttf.h>

#include "Font.hpp"

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"


bae::Font::Font(const std::string& fullPath, const float size) : m_Font(nullptr)
{
    m_Font = TTF_OpenFont(fullPath.c_str(), size);
    if(m_Font == nullptr)
    {
        const std::string errorMessage = "Failed to load font, Error: " + std::string(SDL_GetError());
        Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
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


