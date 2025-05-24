#include "SpriteComponent.h"

#include <SDL.h>

#include "Core/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Core/Renderer.h"


using namespace bae;


SpriteComponent::SpriteComponent(GameObject& owner, const std::string& filename, const SDL_Rect& srcRect, int nrColumns, int nrSprites) :
    TextureComponent(owner, filename),
    m_SrcRect{ srcRect },
    m_DstRect{},
    m_Index{},
    m_NrSprites{ nrSprites },
    m_NrColumns{ nrColumns },
    m_NrRows{ static_cast<int>(std::ceil(static_cast<float>(nrSprites) / nrColumns)) }
{
    if (m_NrColumns == 0 || m_NrRows == 0 || m_NrSprites == 0)
        throw std::runtime_error("SpriteComponent: Ctor: Columns/Rows/Sprites can not be 0");

    SetTexture(filename);
}

void SpriteComponent::Render() const
{
    if (m_Texture)
    {
        const glm::vec3& position = m_Owner->GetWorldLocation();
        const float rotation = m_Owner->GetWorldRotation();
        const glm::vec2& scale = m_Owner->GetWorldScale();

        SDL_Rect srcRect{};

        const int currentColumn = m_Index % m_NrColumns;
        const int currentRow = static_cast<int>(static_cast<float>(m_Index) / m_NrColumns);

        const int spriteWidth = static_cast<int>((static_cast<float>(m_SrcRect.w) / m_NrColumns));
        const int spriteHeight = static_cast<int>((static_cast<float>(m_SrcRect.h) / m_NrRows));

        srcRect.x = currentColumn * spriteWidth;
        srcRect.y = currentRow * spriteHeight;

        srcRect.w = spriteWidth;
        srcRect.h = spriteHeight;

        //std::cout << "column: " << currentColumn << ", row: " << currentRow << '\n';


        SDL_Rect dstRect{};
        dstRect.x = static_cast<int>(position.x - m_DstRect.w / 2.f);
        dstRect.y = static_cast<int>(position.y - m_DstRect.h / 2.f);
        dstRect.w = static_cast<int>(m_DstRect.w);
        dstRect.h = static_cast<int>(m_DstRect.h);

        Renderer::GetInstance().RenderTexture(*m_Texture, srcRect, dstRect, rotation, scale.x, scale.y);
    }
}


void SpriteComponent::PreviousSprite()
{
    if (m_NrSprites == 1)
        return;

    --m_Index;
    if (m_Index < 0)
        m_Index = m_NrSprites - 1;

}

void SpriteComponent::NextSprite()
{
    if (m_NrSprites == 1)
        return;

    m_Index = ++m_Index % m_NrSprites;
}


