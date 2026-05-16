#include "Utils.hpp"

#include <algorithm>

#include <SDL3_gfxPrimitives.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "Core/Renderer.hpp"


bae::Utils::Window::Window(const std::string& title, const std::filesystem::path& resourceFolder, const int width,
                           const int height,
                           const bool isVSyncOn) :
    Title{ title },
    ResourceFolder{ resourceFolder },
    Width{ width },
    Height{ height },
    bIsVSyncOn{ isVSyncOn }
{
}


bool bae::Utils::IsOverlapping(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    return SDL_HasRectIntersection(&rect1, &rect2);
}


Uint8 bae::Utils::FloatToUint8(float v)
{
    v = std::clamp(v, 0.0f, 1.0f);
    return static_cast<Uint8>(std::roundf(v * 255.0f));
}


void bae::Utils::SetDrawColor(const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    SDL_SetRenderDrawColor(pSdlRenderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}


void bae::Utils::DrawLine(const glm::vec2& line1, const glm::vec2& line2, const int width, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    thickLineRGBA(pSdlRenderer, static_cast<Sint16>(line1.x), static_cast<Sint16>(line1.y),
                  static_cast<Sint16>(line2.x), static_cast<Sint16>(line2.y), static_cast<Uint8>(width),
                  sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}


void bae::Utils::DrawRect(const SDL_FRect& rect, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    SetDrawColor(color);
    rectangleRGBA(pSdlRenderer, static_cast<Sint16>(rect.x), static_cast<Sint16>(rect.y),
                  static_cast<Sint16>(rect.x + rect.w), static_cast<Sint16>(rect.y + rect.h),
                  sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}

void bae::Utils::FillRect(const SDL_FRect& rect, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    SetDrawColor(color);

    const SDL_Color sdlColor = color.GetSDLColor();
    boxRGBA(pSdlRenderer, static_cast<Sint16>(rect.x), static_cast<Sint16>(rect.y),
            static_cast<Sint16>(rect.x + rect.w), static_cast<Sint16>(rect.y + rect.h),
            sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}


void bae::Utils::DrawCircle(const glm::vec2& center, const int radius, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    circleRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
               static_cast<Sint16>(radius),
               sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}

void bae::Utils::FillCircle(const glm::vec2& center, const int radius, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    filledCircleRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
                     static_cast<Sint16>(radius),
                     sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}


void bae::Utils::DrawEllipse(const glm::vec2& center, const int radiusX, const int radiusY, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    ellipseRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
                static_cast<Sint16>(radiusX), static_cast<Sint16>(radiusY),
                sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}

void bae::Utils::FillEllipse(const glm::vec2& center, const int radiusX, const int radiusY, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    filledEllipseRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
                      static_cast<Sint16>(radiusX), static_cast<Sint16>(radiusY),
                      sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}


void bae::Utils::DrawPolygon(const std::vector<glm::vec2>& points, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    if(points.size() < 3)
    {
        return;
    }

    std::vector<Sint16> posX;
    std::vector<Sint16> posY;

    for(const auto& point : points)
    {
        posX.push_back(static_cast<Sint16>(point.x));
        posY.push_back(static_cast<Sint16>(point.y));
    }

    const SDL_Color sdlColor = color.GetSDLColor();
    polygonRGBA(pSdlRenderer, posX.data(), posY.data(), static_cast<int>(points.size()),
                sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}

void bae::Utils::FillPolygon(const std::vector<glm::vec2>& points, const Color& color)
{
    SDL_Renderer* pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    if(points.size() < 3)
    {
        return;
    }

    std::vector<Sint16> posX;
    std::vector<Sint16> posY;

    for(const auto& point : points)
    {
        posX.push_back(static_cast<Sint16>(point.x));
        posY.push_back(static_cast<Sint16>(point.y));
    }

    const SDL_Color sdlColor = color.GetSDLColor();

    // This function leaks memory
    filledPolygonRGBA(pSdlRenderer, posX.data(), posY.data(), static_cast<int>(points.size()),
                      sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
}


