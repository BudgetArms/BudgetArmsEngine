#include <stdexcept>
#include <string>
#include <iostream>

#include <glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Renderer.h"
#include "Managers/SceneManager.h"
#include "Wrappers/Texture2D.h"

#include "Core/Utils.h"


namespace bu = bae::Utils;


int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;

	}

	return openglIndex;
}

void bae::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);

	if (m_Renderer == nullptr)
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(Renderer::GetInstance().GetSDLWindow(), SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void bae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().RenderGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
}

void bae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void bae::Renderer::RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position, float angle, const glm::vec2& scale) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(position.x);
	dst.y = static_cast<int>(position.y);

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	dst.w = static_cast<int>(std::abs(scale.x) * dst.w);
	dst.h = static_cast<int>(std::abs(scale.y) * dst.h);

	if (isCenteredAtPosition)
	{
		dst.x -= static_cast<int>(dst.w / 2.f);
		dst.y -= static_cast<int>(dst.h / 2.f);
	}


	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (scale.x < 0.0f && scale.y < 0.0f)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if (scale.y < 0.0f)
		flip = SDL_FLIP_HORIZONTAL;
	else if (scale.y < 0.0f)
		flip = SDL_FLIP_VERTICAL;


	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, NULL, flip);


	if (m_bRenderPosition)
		bae::Utils::FillCircle(position, m_RenderPositionRadius, m_RenderPositionColor);

	if (m_bRenderDstRect)
		bae::Utils::DrawRect(dst, m_RenderDstRectColor);

}

void bae::Renderer::RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(position.x);
	dst.y = static_cast<int>(position.y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	if (isCenteredAtPosition)
	{
		dst.x -= static_cast<int>(dst.w / 2.f);
		dst.y -= static_cast<int>(dst.h / 2.f);
	}

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);


	if (m_bRenderPosition)
		bae::Utils::FillCircle(position, m_RenderPositionRadius, m_RenderPositionColor);

	if (m_bRenderDstRect)
		bae::Utils::DrawRect(dst, m_RenderDstRectColor);

}


void bae::Renderer::RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const SDL_Rect& src, const SDL_Rect& dst, float angle, const glm::vec2& scale) const
{
	isCenteredAtPosition;
	SDL_Rect dstScaled = dst;
	dstScaled.w = static_cast<int>(std::abs(scale.x) * dstScaled.w);
	dstScaled.h = static_cast<int>(std::abs(scale.y) * dstScaled.h);

	if (isCenteredAtPosition)
	{
		dstScaled.x -= static_cast<int>(dstScaled.w / 2.f);
		dstScaled.y -= static_cast<int>(dstScaled.h / 2.f);
	}


	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (scale.x < 0.0f && scale.y < 0.0f)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if (scale.x < 0.0f)
		flip = SDL_FLIP_HORIZONTAL;
	else if (scale.y < 0.0f)
		flip = SDL_FLIP_VERTICAL;

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dstScaled, angle, NULL, flip);


	if (m_bRenderPosition)
		bu::FillCircle({ dst.x, dst.y }, m_RenderPositionRadius, m_RenderPositionColor);

	if (m_bRenderDstRect)
		bu::DrawRect(dstScaled, m_RenderDstRectColor);

}


