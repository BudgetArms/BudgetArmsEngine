#include "SpriteComponent.h"

#include <SDL.h>

#include "Core/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Core/Renderer.h"


using namespace bae;


SpriteComponent::SpriteComponent(GameObject& owner, const std::string& filename, const SDL_Rect& fullSrcRect,
	int nrColumns, int nrSprites, const glm::ivec2& srcOffset) :
	TextureComponent(owner, filename),
	m_SrcRect{ fullSrcRect },
	m_DstRect{},
	m_Index{},
	m_SrcOffset{ srcOffset },
	m_NrSprites{ nrSprites },
	m_NrColumns{ nrColumns },
	m_NrRows{ static_cast<int>(std::ceil(static_cast<float>(nrSprites) / nrColumns)) }
{
	if (m_NrColumns == 0 || m_NrRows == 0 || m_NrSprites == 0)
		throw std::runtime_error("SpriteComponent: Ctor: Columns/Rows/Sprites can not be 0");

	m_DstRect.w = m_SrcRect.w / m_NrColumns - srcOffset.x;
	m_DstRect.h = m_SrcRect.h / m_NrRows - srcOffset.y;

	SetTexture(filename);
}

void SpriteComponent::Render() const
{
	if (!m_Texture)
		return;

	// bad idea, if you want to have a player with several spriteComponents, this wont work
	// or wait, you could use GameObject's Children, but you can't Attach Children in the Component itself
	// because they also need to be added to the scene

	// so you need to add the children in advance
	const glm::vec3& position = m_Owner->GetWorldLocation();


	SDL_Rect dstRect{};
	if (m_bUseParentLocation)
	{
		dstRect.x = static_cast<int>(position.x);
		dstRect.y = static_cast<int>(position.y);
	}
	else
	{
		dstRect.x = static_cast<int>(m_DstRect.w);
		dstRect.y = static_cast<int>(m_DstRect.h);
	}

	dstRect.w = static_cast<int>(m_DstRect.w);
	dstRect.h = static_cast<int>(m_DstRect.h);

	const float rotation = m_Owner->GetWorldRotation();
	const glm::vec2& scale = m_Owner->GetWorldScale();
	Renderer::GetInstance().RenderTexture(*m_Texture, m_bIsCenteredAtPosition, GetCurrentSpriteRect(), dstRect, rotation, scale);
}

SDL_Rect SpriteComponent::GetCurrentSpriteRect() const
{
	SDL_Rect srcRect{};

	const int currentColumn = m_Index % m_NrColumns;
	const int currentRow = static_cast<int>(static_cast<float>(m_Index) / m_NrColumns);

	const int spriteWidth = static_cast<int>(static_cast<float>(m_SrcRect.w - m_NrColumns * m_SrcOffset.x) / m_NrColumns);
	const int spriteHeight = static_cast<int>(static_cast<float>(m_SrcRect.h - m_NrRows * m_SrcOffset.y) / m_NrRows);

	srcRect.x = static_cast<int>(m_SrcRect.x) + currentColumn * (spriteWidth + m_SrcOffset.x);
	srcRect.y = static_cast<int>(m_SrcRect.y) + currentRow * (spriteHeight + m_SrcOffset.y);

	srcRect.x = static_cast<int>(m_SrcRect.x) + currentColumn * (spriteWidth + m_SrcOffset.x) + m_SrcOffset.x;
	srcRect.y = static_cast<int>(m_SrcRect.y) + currentRow * (spriteHeight + m_SrcOffset.y) + m_SrcOffset.y;

	srcRect.w = spriteWidth;
	srcRect.h = spriteHeight;

	return srcRect;
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


