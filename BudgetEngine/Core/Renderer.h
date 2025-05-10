#pragma once
#include <SDL.h>
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

        void RenderTexture(const Texture2D& texture, float x, float y, float angle = 0.f, float scaleX = 1.f, float scaleY = 1.f) const;
        void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

        SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

        const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
        void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

    private:
        SDL_Renderer* m_Renderer{};
        SDL_Window* m_Window{};
        SDL_Color m_ClearColor{};

    };
}

