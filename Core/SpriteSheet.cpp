#include "SpriteSheet.hpp"

// used for std::terminate
#include <exception>

#include "Core/HelperFunctions.hpp"
#include "Core/Logger.hpp"
#include "Core/Renderer.hpp"
#include "Managers/ResourceManager.hpp"


using namespace bae;


SpriteSheet::SpriteSheet(const std::filesystem::path& file, const SDL_FRect& srcRect, const int nrColumns,
                         const int nrSprites, const glm::ivec2& srcOffset) :
    m_SrcRect{ srcRect },
    m_SrcOffset{ srcOffset },
    m_NrSprites{ nrSprites },
    m_NrColumns{ nrColumns }
{
    const auto resourceFolder = ResourceManager::GetInstance().GetResourcesPath();
    if(!std::filesystem::exists(resourceFolder / file))
    {
        Logger::LogFatal("File Not Found, Path: " + std::filesystem::path(resourceFolder / file).string());
        std::terminate();
    }

    if(m_NrSprites == 0)
    {
        Logger::LogFatal("NrSprites cannot be 0");
        std::terminate();
    }

    if(m_NrColumns == 0)
    {
        Logger::LogFatal("NrColumns cannot be 0");
        std::terminate();
    }

    m_NrRows = static_cast<int>(std::ceil(static_cast<float>(m_NrSprites) / static_cast<float>(m_NrColumns)));
    if(m_NrRows == 0)
    {
        Logger::LogFatal("NrRows cannot be 0");
        std::terminate();
    }

    m_Texture = ResourceManager::GetInstance().LoadTexture(file.string());
    if(!m_Texture)
    {
        Logger::LogFatal("Failed to load texture");
        std::terminate();
    }

    m_DstRect.w = m_SrcRect.w / static_cast<float>(m_NrColumns) - static_cast<float>(srcOffset.x);
    m_DstRect.h = m_SrcRect.h / static_cast<float>(m_NrRows) - static_cast<float>(srcOffset.y);
}

void SpriteSheet::Render() const
{
    if(!m_Texture)
    {
        return;
    }

    SDL_FRect dstRect
    {
        .x = -1,
        .y = -1,
        .w = m_DstRect.w,
        .h = m_DstRect.h,
    };

    if(m_bUseParentLocation)
    {
        dstRect.x = m_Position.x;
        dstRect.y = m_Position.y;
    }
    else
    {
        dstRect.x = m_DstRect.w;
        dstRect.y = m_DstRect.h;
    }


    const float rotation   = m_Rotation;
    const glm::vec2& scale = m_Scale;

    Renderer::GetInstance().RenderTexture(
        *m_Texture,
        m_bIsCenteredAtPosition,
        GetCurrentSpriteRect(),
        dstRect,
        rotation,
        scale);
}

SDL_FRect SpriteSheet::GetCurrentSpriteRect() const
{
    const int currentColumn = m_Index % m_NrColumns;
    const int currentRow    = static_cast<int>(static_cast<float>(m_Index) / static_cast<float>(m_NrColumns));

    const float spriteWidth = (m_SrcRect.w - static_cast<float>(m_NrColumns) * m_SrcOffset.x) / static_cast<float>(
        m_NrColumns);
    const float spriteHeight = (m_SrcRect.h - static_cast<float>(m_NrRows) * m_SrcOffset.y) / static_cast<float>(
        m_NrRows);


    return SDL_FRect
    {
        .x = m_SrcRect.x + static_cast<float>(currentColumn) * (spriteWidth + m_SrcOffset.x) + m_SrcOffset.x,
        .y = m_SrcRect.y + static_cast<float>(currentRow) * (spriteHeight + m_SrcOffset.y) + m_SrcOffset.y,
        .w = spriteWidth,
        .h = spriteHeight
    };
}

void SpriteSheet::PreviousSprite()
{
    if(m_NrSprites == 1)
    {
        return;
    }

    --m_Index;
    if(m_Index < 0)
    {
        m_Index = m_NrSprites - 1;
    }
}

void SpriteSheet::NextSprite()
{
    if(m_NrSprites == 1)
    {
        return;
    }

    ++m_Index;
    m_Index %= m_NrSprites;
}
