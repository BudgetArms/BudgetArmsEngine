#pragma once

#if WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>

#include "Color.hpp"
#include "Logger.hpp"


namespace bae::Utils
{
    struct Window
    {
        explicit Window(const std::string& title, const std::filesystem::path& resourceFolder, int width, int height,
                        bool isVSyncOn);


        std::string Title;
        std::filesystem::path ResourceFolder;
        int Width;
        int Height;
        bool bIsVSyncOn;
    };

    
    void CreateConsole();

    // Remove Window's ReplaceText Macro
    #ifdef ReplaceText
    #undef ReplaceText
    #endif
    [[nodiscard]] std::string ReplaceText(const std::string_view& inputString,
                                          const std::string_view& oldText,
                                          const std::string_view& newText);


    bool IsOverlapping(const SDL_Rect& rect1, const SDL_Rect& rect2);

    Uint8 FloatToUint8(float v);

    void SetDrawColor(const Color& color);

    void DrawLine(const glm::vec2& line1, const glm::vec2& line2, int width, const Color& color);

    void DrawRect(const SDL_FRect& rect, const Color& color);
    void FillRect(const SDL_FRect& rect, const Color& color);

    void DrawCircle(const glm::vec2& center, int radius, const Color& color);
    void FillCircle(const glm::vec2& center, int radius, const Color& color);

    void DrawEllipse(const glm::vec2& center, int radiusX, int radiusY, const Color& color);
    void FillEllipse(const glm::vec2& center, int radiusX, int radiusY, const Color& color);

    void DrawPolygon(const std::vector<glm::vec2>& points, const Color& color);
    void FillPolygon(const std::vector<glm::vec2>& points, const Color& color);
}
