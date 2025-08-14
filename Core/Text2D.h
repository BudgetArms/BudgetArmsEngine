#pragma once

#include <memory>
#include <string.h>

#include <SDL.h>

#include "Wrappers/Texture2D.h"
#include "Wrappers/Font.h"


namespace bae
{
	class Text2D
	{
	public:
		Text2D(const std::string& text, std::shared_ptr<Font> font = nullptr, const SDL_Color& color = SDL_Color{ 255, 255, 255, 255 });
		virtual ~Text2D() = default;

		Text2D(const Text2D& other) = delete;
		Text2D(Text2D&& other) = delete;
		Text2D& operator=(const Text2D& other) = delete;
		Text2D& operator=(Text2D&& other) = delete;


		void Update();
		void Render() const;


		std::string GetText();
		void SetText(const std::string& text);

		SDL_Color GetColor();
		void SetColor(const SDL_Color& color);

		Font* GetFont() const { return m_uFont.get(); };
		void SetFont(std::shared_ptr<Font> font);


		bool m_bIsCenteredAtPosition;
		glm::vec2 m_Position;
		float m_Rotation;
		glm::vec2 m_Scale;


	private:

		std::shared_ptr<Font> m_uFont;
		std::unique_ptr<Texture2D> m_uTextTexture;

		bool m_bNeedsUpdate;
		std::string m_Text;
		SDL_Color m_Color;


	};
}


