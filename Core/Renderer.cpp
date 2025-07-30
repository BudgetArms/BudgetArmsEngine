#include <stdexcept>
#include <string>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Renderer.h"
#include "Managers/SceneManager.h"
#include "Wrappers/Texture2D.h"


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

void bae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, float angle, float scaleX, float scaleY) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);

	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	dst.w = static_cast<int>(scaleX * dst.w);
	dst.h = static_cast<int>(scaleY * dst.h);


	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (scaleX < 0.0f && scaleY < 0.0f)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if (scaleX < 0.0f)
		flip = SDL_FLIP_HORIZONTAL;
	else if (scaleY < 0.0f)
		flip = SDL_FLIP_VERTICAL;


	const SDL_Point center = { dst.w / 2,  dst.h / 2 };
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, &center, flip);
}

void bae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}


void bae::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect& src, const SDL_Rect& dst, float angle, float scaleX, float scaleY) const
{
	SDL_Rect dstScaled = dst;
	dstScaled.w *= static_cast<int>(scaleX);
	dstScaled.h *= static_cast<int>(scaleY);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (scaleX < 0.0f && scaleY < 0.0f)
		flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else if (scaleX < 0.0f)
		flip = SDL_FLIP_HORIZONTAL;
	else if (scaleY < 0.0f)
		flip = SDL_FLIP_VERTICAL;

	//const SDL_Point center = { dstScaled.w / 2,  dstScaled.h / 2 };
	const SDL_Point center = { dst.w / 2,  dst.h / 2 };
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dstScaled, angle, &center, flip);
}


