#include "Utils.h"

#include <algorithm>
#include <cmath>

#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL2_gfxPrimitives.h>

#include "Core/Renderer.h"


namespace bu = bae::Utils;


bu::Window::Window(const std::string& title, const std::filesystem::path& resourceFolder, int width, int height, bool isVSyncOn) :
	title{ title },
	resourceFolder{ resourceFolder },
	width{ width },
	height{ height },
	bIsVSyncOn{ isVSyncOn }
{
}


bu::Color::Color(float r, float g, float b, float a) :
	r{ r },
	g{ g },
	b{ b },
	a{ a }
{
}

bu::Color::Color(const bu::Color& other, float a) :
	bu::Color(other.r, other.g, other.b, a)
{
}

bu::Color::Color() :
	r{ 0.f },
	g{ 0.f },
	b{ 0.f },
	a{ 1.f }
{
}


const bu::Color bu::Color::White(1.f, 1.f, 1.f);
const bu::Color bu::Color::Black(0.f, 0.f, 0.f);
const bu::Color bu::Color::Red(1.f, 0.f, 0.f);
const bu::Color bu::Color::Green(0.f, 1.f, 0.f);
const bu::Color bu::Color::Blue(0.f, 0.f, 1.f);
const bu::Color bu::Color::Yellow(1.f, 1.f, 0.f);
const bu::Color bu::Color::Orange(1.f, 0.392f, 0.f);
const bu::Color bu::Color::Magenta(1.f, 0.f, 1.f);
const bu::Color bu::Color::Cyan(0.f, 1.f, 1.f);
const bu::Color bu::Color::Purple(0.392f, 0.165f, 0.624f);
const bu::Color bu::Color::Gray(0.753f, 0.753f, 0.753f);
const bu::Color bu::Color::DarkGray(0.212f, 0.231f, 0.247f);
const bu::Color bu::Color::Transparent(0.f, 0.f, 0.f, 0.f);


bool bu::IsOverlapping(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	return static_cast<bool>(SDL_HasIntersection(&rect1, &rect2));
}


Uint8 bu::FloatToUint8(float v)
{
	v = std::clamp(v, 0.0f, 1.0f);
	return static_cast<Uint8>(std::roundf(v * 255.0f));
}


void bu::SetDrawColor(const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	SDL_SetRenderDrawColor(pSdlRenderer,
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bu::DrawLine(const glm::vec2& line1, const glm::vec2& line2, int width, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	GFX_thickLineRGBA(pSdlRenderer, static_cast<Sint16>(line1.x), static_cast<Sint16>(line1.y),
		static_cast<Sint16>(line2.x), static_cast<Sint16>(line2.y), static_cast<Uint8>(width),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bu::DrawRect(const SDL_Rect& rect, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	SetDrawColor(color);
	GFX_rectangleRGBA(pSdlRenderer, static_cast<Sint16>(rect.x), static_cast<Sint16>(rect.y),
		static_cast<Sint16>(rect.x + rect.w), static_cast<Sint16>(rect.y + rect.h),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));

}

void bu::FillRect(const SDL_Rect& rect, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	SetDrawColor(color);
	GFX_boxRGBA(pSdlRenderer, static_cast<Sint16>(rect.x), static_cast<Sint16>(rect.y),
		static_cast<Sint16>(rect.x + rect.w), static_cast<Sint16>(rect.y + rect.h),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}


void bu::DrawCircle(const glm::vec2& center, int radius, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	GFX_circleRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
		static_cast<Sint16>(radius),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));
}

void bu::FillCircle(const glm::vec2& center, int radius, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	GFX_filledCircleRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
		static_cast<Sint16>(radius),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));

}


void bu::DrawEllipse(const glm::vec2& center, int radiusX, int radiusY, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	GFX_ellipseRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
		static_cast<Sint16>(radiusX), static_cast<Sint16>(radiusY),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));

}

void bu::FillEllipse(const glm::vec2& center, int radiusX, int radiusY, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	GFX_filledEllipseRGBA(pSdlRenderer, static_cast<Sint16>(center.x), static_cast<Sint16>(center.y),
		static_cast<Sint16>(radiusX), static_cast<Sint16>(radiusY),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));

}


void bu::DrawPolygon(const std::vector<glm::vec2>& points, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	if (points.size() < 3)
		return;

	std::vector<Sint16> posX;
	std::vector<Sint16> posY;

	for (const auto& point : points)
	{
		posX.push_back(static_cast<Sint16>(point.x));
		posY.push_back(static_cast<Sint16>(point.y));
	}

	GFX_polygonRGBA(pSdlRenderer, posX.data(), posY.data(), static_cast<int>(points.size()),
		FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));

}

void bu::FillPolygon(const std::vector<glm::vec2>& points, const Color& color)
{
	auto const pSdlRenderer = bae::Renderer::GetInstance().GetSDLRenderer();
	if (!pSdlRenderer)
		return;

	if (points.size() < 3)
		return;

	std::vector<Sint16> posX;
	std::vector<Sint16> posY;

	for (const auto& point : points)
	{
		posX.push_back(static_cast<Sint16>(point.x));
		posY.push_back(static_cast<Sint16>(point.y));
	}

	color;
	//GFX_filledPolygonRGBA(pSdlRenderer, posX.data(), posY.data(), static_cast<int>(points.size()),
		//FloatToUint8(color.r), FloatToUint8(color.g), FloatToUint8(color.b), FloatToUint8(color.a));

}


