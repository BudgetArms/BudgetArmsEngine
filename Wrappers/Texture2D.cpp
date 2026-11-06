#include "Texture2D.hpp"

#include <stdexcept>

#include <SDL3/SDL.h>

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"
#include "Core/Renderer.hpp"


bae::Texture2D::Texture2D(SDL_Texture* texture) :
    m_Texture{ texture }
{
    if(!m_Texture)
    {
        const std::string errorMessage = "Invalid texture";
        Logger::LogError(errorMessage);
        assert(false && (FUNCTION_NAME + errorMessage).c_str());
    }
}


bae::Texture2D::Texture2D(const std::string& fullPath)
{
    SDL_Surface* surface = SDL_LoadPNG(fullPath.c_str());
    if(!surface)
    {
        const std::string errorMessage = "Failed to load PNG, Error" + std::string(SDL_GetError());
        Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
    }

    m_Texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        surface
    );

    SDL_DestroySurface(surface);

    if(!m_Texture)
    {
        const std::string errorMessage = "Failed to create texture from surface, Error: " + std::string(SDL_GetError());
        Logger::LogError(errorMessage);
        throw std::runtime_error(FUNCTION_NAME + errorMessage);
    }
}

bae::Texture2D::~Texture2D()
{
    SDL_DestroyTexture(m_Texture);
}


glm::vec2 bae::Texture2D::GetSize() const
{
    float width{};
    float height{};
    SDL_GetTextureSize(m_Texture, &width, &height);
    return { width, height };
}

SDL_Texture* bae::Texture2D::GetSDLTexture() const
{
    return m_Texture;
}


