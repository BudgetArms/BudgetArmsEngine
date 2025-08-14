#pragma once

#include "Components/Component.h"


namespace bae
{
	class GameObject;
	class TextureComponent : public Component
	{
	public:
		explicit TextureComponent(GameObject& owner, const std::string& filename);


		virtual void Render() const override;

		void SetTexture(const std::string& filename);


		bool m_bIsCenteredAtPosition{ false };


	protected:
		std::shared_ptr<Texture2D> m_Texture{};


	};
}


