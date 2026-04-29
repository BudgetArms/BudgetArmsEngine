#include "Texture2D.h"

#include <stdexcept>

#include <SDL3/SDL.h>

#include "Core/HelperFunctions.h"
#include "Core/Renderer.h"


bae::Texture2D::Texture2D(SDL_Texture* texture) :
    m_Texture{ texture }
{
    assert(m_Texture != nullptr);
}

bae::Texture2D::Texture2D(const std::string& fullPath)
{
    SDL_Surface* surface = SDL_LoadPNG(fullPath.c_str());
    if(!surface)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to load PNG, Error: ") + SDL_GetError());
    }

    m_Texture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        surface
    );

    SDL_DestroySurface(surface);

    if(!m_Texture)
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to create texture from surface, Error: ") + SDL_GetError());
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


