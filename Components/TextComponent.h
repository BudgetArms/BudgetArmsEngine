#pragma once
#include <string>
#include <memory>

#include <SDL_pixels.h>

#include "Core/GameObject.h"
#include "Components/TransformComponent.h"


namespace bae
{
	class Font;
	class Texture2D;
	class TextComponent : public Component
	{
	public:
		TextComponent(GameObject& owner, const std::string& text, std::shared_ptr<Font> font = nullptr, SDL_Color color = SDL_Color{ 255, 255, 255, 255 });
		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;


		virtual void Update() override;
		virtual void Render() const override;

		std::string GetText();
		void SetText(const std::string& text);

		SDL_Color GetColor();
		void SetColor(const SDL_Color& color);

		Font* GetFont() const { return m_Font.get(); };
		void SetFont(std::shared_ptr<Font> font);


		bool m_bIsCenteredAtPosition{ false };


	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		SDL_Color m_Color;
		std::shared_ptr<Font> m_Font;


	protected:
		std::unique_ptr<Texture2D> m_TextTexture;


	};
}


