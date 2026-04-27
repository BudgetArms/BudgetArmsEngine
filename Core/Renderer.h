#pragma once

#include <glm/glm.hpp>
#include <SDL3/SDL.h>

#include "Core/Utils.h"
#include "Singletons/Singleton.h"


namespace bae
{
    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    struct WindowSize
    {
        int Width{};
        int Height{};
    };

    class Renderer final : public Singleton<Renderer>
    {
    public:
        void Init(SDL_Window* window);
        void Render() const;
        void Destroy();

        void RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position, float angle,
                           const glm::vec2& scale = { 1.f, 1.f }) const;
        void RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position, float width,
                           float height) const;
        void RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const SDL_FRect& src,
                           const SDL_FRect& dst, float angle = 0.f, const glm::vec2& scale = { 1.f, 1.f }) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
        [[nodiscard]] SDL_Window* GetSDLWindow() const { return m_Window; }
        [[nodiscard]] WindowSize GetSDLWindowSize() const;

        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
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


