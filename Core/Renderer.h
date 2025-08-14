#pragma once

#include <glm.hpp>
#include <SDL.h>

#include "Core/Utils.h"
#include "Singletons/Singleton.h"




namespace bae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position, float angle, const glm::vec2& scale = { 1.f, 1.f }) const;
		void RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position, float width, float height) const;
		void RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const SDL_Rect& src, const SDL_Rect& dst, float angle = 0.f, const glm::vec2& scale = { 1.f, 1.f }) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
		SDL_Window* GetSDLWindow() const { return m_Window; }

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }


		bool m_bRenderPosition{ false };
		bool m_bRenderDstRect{ false };

		int m_RenderPositionRadius{ 5 };
		bae::Utils::Color m_RenderPositionColor{ bae::Utils::Color::Red };
		bae::Utils::Color m_RenderDstRectColor{ bae::Utils::Color::Green };


	private:
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};


	};
}


