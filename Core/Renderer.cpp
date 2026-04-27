#include <stdexcept>
#include <string>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <glm/glm.hpp>


#include "Renderer.h"
#include "Managers/SceneManager.h"
#include "Wrappers/Texture2D.h"

#include "Core/Utils.h"


namespace bu = bae::Utils;


void bae::Renderer::Init(SDL_Window* window)
{
    m_Window = window;

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    #if defined(__EMSCRIPTEN__)
    m_Renderer = SDL_CreateRenderer(window, nullptr);
    #else
    m_Renderer = SDL_CreateRenderer(window, nullptr);
    #endif

    if(m_Renderer == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const bool success = ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer3_Init(m_Renderer);
    if(!success)
    {
        throw std::runtime_error("ImGui_ImplSDL3_InitForSDLRenderer: failed to initialize");
    }
}

void bae::Renderer::Render() const
{
    const SDL_Color& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_Renderer);

    SceneManager::GetInstance().Render();

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    SceneManager::GetInstance().RenderGUI();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);

    SDL_RenderPresent(m_Renderer);
}

void bae::Renderer::Destroy()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if(m_Renderer != nullptr)
    {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }
}

void bae::Renderer::RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position,
                                  float angle, const glm::vec2& scale) const
{
    SDL_FRect dst{};
    dst.x = position.x;
    dst.y = position.y;

    SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);

    dst.w = std::abs(scale.x) * dst.w;
    dst.h = std::abs(scale.y) * dst.h;

    if(isCenteredAtPosition)
    {
        dst.x -= dst.w / 2.f;
        dst.y -= dst.h / 2.f;
    }


    SDL_FlipMode flip = SDL_FLIP_NONE;

    if(scale.x < 0.0f && scale.y < 0.0f)
    {
        flip = static_cast<SDL_FlipMode>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }
    else if(scale.y < 0.0f)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    else if(scale.y < 0.0f)
    {
        flip = SDL_FLIP_VERTICAL;
    }


    SDL_RenderTextureRotated(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, nullptr, flip);


    if(m_bRenderPosition)
    {
        bae::Utils::FillCircle(position, m_RenderPositionRadius, m_RenderPositionColor);
    }

    if(m_bRenderDstRect)
    {
        bae::Utils::DrawRect(dst, m_RenderDstRectColor);
    }
}

void bae::Renderer::RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const glm::vec2& position,
                                  const float width, const float height) const
{
    SDL_FRect dst{};
    dst.x = position.x;
    dst.y = position.y;
    dst.w = width;
    dst.h = height;

    if(isCenteredAtPosition)
    {
        dst.x -= dst.w / 2.f;
        dst.y -= dst.h / 2.f;
    }

    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);


    if(m_bRenderPosition)
    {
        bae::Utils::FillCircle(position, m_RenderPositionRadius, m_RenderPositionColor);
    }

    if(m_bRenderDstRect)
    {
        bae::Utils::DrawRect(dst, m_RenderDstRectColor);
    }
}


void bae::Renderer::RenderTexture(const Texture2D& texture, bool isCenteredAtPosition, const SDL_FRect& src,
                                  const SDL_FRect& dst, float angle, const glm::vec2& scale) const
{
    SDL_FRect dstScaled = dst;
    dstScaled.w         = std::abs(scale.x) * dstScaled.w;
    dstScaled.h         = std::abs(scale.y) * dstScaled.h;

    if(isCenteredAtPosition)
    {
        dstScaled.x -= dstScaled.w / 2.f;
        dstScaled.y -= dstScaled.h / 2.f;
    }


    SDL_FlipMode flip = SDL_FLIP_NONE;

    if(scale.x < 0.0f && scale.y < 0.0f)
    {
        flip = static_cast<SDL_FlipMode>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }
    else if(scale.x < 0.0f)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
    else if(scale.y < 0.0f)
    {
        flip = SDL_FLIP_VERTICAL;
    }

    SDL_RenderTextureRotated(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dstScaled, angle, nullptr, flip);


    if(m_bRenderPosition)
    {
        bu::FillCircle({ dst.x, dst.y }, m_RenderPositionRadius, m_RenderPositionColor);
    }

    if(m_bRenderDstRect)
    {
        bu::DrawRect(dstScaled, m_RenderDstRectColor);
    }
}

bae::WindowSize bae::Renderer::GetSDLWindowSize() const
{
    int width{};
    int height{};
    SDL_GetWindowSize(m_Window, &width, &height);

    return { width, height };
}


