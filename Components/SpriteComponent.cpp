#include "SpriteComponent.h"

#include "Core/GameObject.h"
#include "Core/HelperFunctions.h"
#include "Core/Renderer.h"
#include "Managers/ResourceManager.h"


using namespace bae;


SpriteComponent::SpriteComponent(GameObject& owner, const std::string& filename, const SDL_FRect& srcRect,
                                 const int nrColumns, const int nrSprites, const glm::ivec2& srcOffset) :
    TextureComponent(owner, filename),
    m_SrcOffset{ srcOffset },
    m_SrcRect{ srcRect },
    m_DstRect{},
    m_NrSprites{ nrSprites },
    m_NrColumns{ nrColumns },
    m_NrRows{ static_cast<int>(std::ceil(static_cast<float>(m_NrSprites) / static_cast<float>(m_NrColumns))) }
{
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

    m_DstRect.w = m_SrcRect.w / static_cast<float>(m_NrColumns) - static_cast<float>(srcOffset.x);
    m_DstRect.h = m_SrcRect.h / static_cast<float>(m_NrRows) - static_cast<float>(srcOffset.y);

    SetTexture(filename);
}

void SpriteComponent::Render() const
{
    if(!m_Texture)
    {
        return;
    }

    // bad idea, if you want to have a player with several spriteComponents, this will not work
    // or wait, you could use GameObject's Children, but you can't Attach Children in the Component itself
    // because they also need to be added to the scene

    // so you need to add the children in advance
    const glm::vec2& position = m_Owner->GetWorldLocation();


    SDL_FRect dstRect{};
    if(m_bUseParentLocation)
    {
        dstRect.x = position.x;
        dstRect.y = position.y;
    }
    else
    {
        dstRect.x = m_DstRect.w;
        dstRect.y = m_DstRect.h;
    }

    dstRect.w = m_DstRect.w;
    dstRect.h = m_DstRect.h;

    const float rotation   = m_Owner->GetWorldRotation();
    const glm::vec2& scale = m_Owner->GetWorldScale();
    Renderer::GetInstance().RenderTexture(*m_Texture, m_bIsCenteredAtPosition, GetCurrentSpriteRect(), dstRect,
                                          rotation, scale);
}

SDL_FRect SpriteComponent::GetCurrentSpriteRect() const
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


void SpriteComponent::PreviousSprite()
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

void SpriteComponent::NextSprite()
{
    if(m_NrSprites == 1)
    {
        return;
    }

    ++m_Index;
    m_Index %= m_NrSprites;
}


