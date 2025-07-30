#include "TextureComponent.h"

#include <SDL.h>

#include "Core/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Core/Renderer.h"


using namespace bae;


TextureComponent::TextureComponent(GameObject& owner, const std::string& filename) :
	Component(owner)
{
	SetTexture(filename);
}

void TextureComponent::Render() const
{
	if (m_Texture)
	{
		const glm::vec3& position = m_Owner->GetWorldLocation();
		const float rotation = m_Owner->GetWorldRotation();
		const glm::vec2& scale = m_Owner->GetWorldScale();

		Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y, rotation, scale.x, scale.y);
	}
}

void TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}


