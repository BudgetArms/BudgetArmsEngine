#include "SpriteSheet.hpp"

#include "HelperFunctions.hpp"
#include "Renderer.hpp"
#include "Managers/ResourceManager.hpp"


using namespace bae;


SpriteSheet::SpriteSheet(const std::filesystem::path& file, const SDL_FRect& srcRect, const int nrColumns,
                         const int nrSprites, const glm::ivec2& srcOffset) :
    m_SrcOffset{ srcOffset },
    m_SrcRect{ srcRect },
    m_NrSprites{ nrSprites },
    m_NrColumns{ nrColumns },
    m_NrRows{ static_cast<int>(std::ceil(static_cast<float>(m_NrSprites) / static_cast<float>(m_NrColumns))) }
{
    const auto resourceFolder = ResourceManager::GetInstance().GetResourcesPath();
    if(!std::filesystem::exists(resourceFolder / file))
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed! File Not found! File Path: ") + file.string());
    }

    if(m_NrColumns == 0)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! NrColumns is 0"));
    }
    if(m_NrRows == 0)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! NrRows is 0"));
    }
    if(m_NrSprites == 0)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! NrSprites is 0"));
    }

    m_Texture = ResourceManager::GetInstance().LoadTexture(file.string());
    if(!m_Texture)
    {
        throw std::runtime_error(FUNCTION_NAME + std::string(" Failed to Load Texture!"));
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

    SDL_FRect dstRect{};
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

    dstRect.w = m_DstRect.w;
    dstRect.h = m_DstRect.h;

    const float rotation   = m_Rotation;
    const glm::vec2& scale = m_Scale;

    Renderer::GetInstance().RenderTexture(*m_Texture, m_bIsCenteredAtPosition, GetCurrentSpriteRect(), dstRect,
                                          rotation, scale);
}

SDL_FRect SpriteSheet::GetCurrentSpriteRect() const
{
    SDL_FRect srcRect{};

    const int currentColumn = m_Index % m_NrColumns;
    const int currentRow    = static_cast<int>(static_cast<float>(m_Index) / static_cast<float>(m_NrColumns));

    const float spriteWidth = (m_SrcRect.w - static_cast<float>(m_NrColumns) * m_SrcOffset.x) / static_cast<float>(
        m_NrColumns);
    const float spriteHeight = (m_SrcRect.h - static_cast<float>(m_NrRows) * m_SrcOffset.y) / static_cast<float>(
        m_NrRows);

    srcRect.x = m_SrcRect.x + static_cast<float>(currentColumn) * (spriteWidth + m_SrcOffset.x);
    srcRect.y = m_SrcRect.y + static_cast<float>(currentRow) * (spriteHeight + m_SrcOffset.y);

    srcRect.x = m_SrcRect.x + static_cast<float>(currentColumn) * (spriteWidth + m_SrcOffset.x) + m_SrcOffset.x;
    srcRect.y = m_SrcRect.y + static_cast<float>(currentRow) * (spriteHeight + m_SrcOffset.y) + m_SrcOffset.y;

    srcRect.w = spriteWidth;
    srcRect.h = spriteHeight;

    return srcRect;
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

