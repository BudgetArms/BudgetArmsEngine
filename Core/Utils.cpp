#include "Utils.h"

#include <algorithm>

#include <SDL3_gfxPrimitives.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "Core/Renderer.h"


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


bae::Utils::Color::Color(const float r, const float g, const float b, const float a) :
    r{ r },
    g{ g },
    b{ b },
    a{ a }
{
}

bae::Utils::Color::Color(const Color& other, const float a) :
    Color(other.r, other.g, other.b, a)
{
}

bae::Utils::Color::Color() :
    r{ 0.f },
    g{ 0.f },
    b{ 0.f },
    a{ 1.f }
{
}


const bae::Utils::Color bae::Utils::Color::White(1.f, 1.f, 1.f);
const bae::Utils::Color bae::Utils::Color::Black(0.f, 0.f, 0.f);
const bae::Utils::Color bae::Utils::Color::Red(1.f, 0.f, 0.f);
const bae::Utils::Color bae::Utils::Color::Green(0.f, 1.f, 0.f);
const bae::Utils::Color bae::Utils::Color::Blue(0.f, 0.f, 1.f);
const bae::Utils::Color bae::Utils::Color::Yellow(1.f, 1.f, 0.f);
const bae::Utils::Color bae::Utils::Color::Orange(1.f, 0.392f, 0.f);
const bae::Utils::Color bae::Utils::Color::Magenta(1.f, 0.f, 1.f);
const bae::Utils::Color bae::Utils::Color::Cyan(0.f, 1.f, 1.f);
const bae::Utils::Color bae::Utils::Color::Purple(0.392f, 0.165f, 0.624f);
const bae::Utils::Color bae::Utils::Color::Gray(0.753f, 0.753f, 0.753f);
const bae::Utils::Color bae::Utils::Color::DarkGray(0.212f, 0.231f, 0.247f);
const bae::Utils::Color bae::Utils::Color::Transparent(0.f, 0.f, 0.f, 0.f);


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

    SDL_SetRenderDrawColor(pSdlRenderer,
                           FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bae::Utils::DrawLine(const glm::vec2& line1, const glm::vec2& line2, const int width, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    thickLineRGBA(pSdlRenderer, static_cast<Sint16>(line1.x), static_cast<Sint16>(line1.y),
                  static_cast<Sint16>(line2.x), static_cast<Sint16>(line2.y), static_cast<Uint8>(width),
                  FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bae::Utils::DrawRect(const SDL_FRect& rect, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    SetDrawColor(color);
    rectangleRGBA(pSdlRenderer, static_cast<Sint16>(rect.x), static_cast<Sint16>(rect.y),
                  static_cast<Sint16>(rect.x + rect.w), static_cast<Sint16>(rect.y + rect.h),
                  FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}

void bae::Utils::FillRect(const SDL_FRect& rect, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    SetDrawColor(color);
    boxRGBA(pSdlRenderer, static_cast<Sint16>(rect.x), static_cast<Sint16>(rect.y),
            static_cast<Sint16>(rect.x + rect.w), static_cast<Sint16>(rect.y + rect.h),
            FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bae::Utils::DrawCircle(const glm::vec2& center, const int radius, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    circleRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
               static_cast<Sint16>(radius),
               FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}

void bae::Utils::FillCircle(const glm::vec2& center, const int radius, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    filledCircleRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
                     static_cast<Sint16>(radius),
                     FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bae::Utils::DrawEllipse(const glm::vec2& center, const int radiusX, const int radiusY, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    ellipseRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
                static_cast<Sint16>(radiusX), static_cast<Sint16>(radiusY),
                FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}

void bae::Utils::FillEllipse(const glm::vec2& center, const int radiusX, const int radiusY, const Color& color)
{
    auto const pSdlRenderer = Renderer::GetInstance().GetSDLRenderer();
    if(!pSdlRenderer)
    {
        return;
    }

    filledEllipseRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
                      static_cast<Sint16>(radiusX), static_cast<Sint16>(radiusY),
                      FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
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

    polygonRGBA(pSdlRenderer, posX.data(), posY.data(), static_cast<int>(points.size()),
                FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
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

    // This function leaks memory
    filledPolygonRGBA(pSdlRenderer, posX.data(), posY.data(), static_cast<int>(points.size()),
                      FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


