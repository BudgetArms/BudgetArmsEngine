#include "TextureComponent.hpp"

#include "Core/GameObject.hpp"
#include "Core/Renderer.hpp"
#include "Managers/ResourceManager.hpp"
#include "Wrappers/Texture2D.hpp"


using namespace bae;


TextureComponent::TextureComponent(GameObject& owner, const std::string& filename) :
    Component(owner)
{
    SetTexture(filename);
}

void TextureComponent::Render() const
{
    if(m_Texture)
    {
        const glm::vec2& position = m_Owner->GetWorldLocation();
        const float rotation      = m_Owner->GetWorldRotation();
        const glm::vec2& scale    = m_Owner->GetWorldScale();

        Renderer::GetInstance().RenderTexture(*m_Texture, m_bIsCenteredAtPosition, position, rotation, scale);
    }
}

void TextureComponent::SetTexture(const std::string& filename)
{
    m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}


