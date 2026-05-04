#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>


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


    struct Color
    {
        // [0-1]
        // constexpr explicit Color(const float r, const float g, const float b, const float a = 1.f) :
        constexpr explicit Color(const float r, const float g, const float b, const float a = 1.f) :
            R{ std::clamp(r, 0.f, 1.f) },
            G{ std::clamp(g, 0.f, 1.f) },
            B{ std::clamp(b, 0.f, 1.f) },
            A{ std::clamp(a, 0.f, 1.f) }
        {
        }

        constexpr Color(const Color& other, const float a) :
            Color(other.R, other.G, other.B, a)
        {
        }

        [[nodiscard]] constexpr SDL_Color GetSDLColor() const;

        float R{};
        float G{};
        float B{};
        float A{};


        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Orange;
        static const Color Magenta;
        static const Color Cyan;
        static const Color Purple;
        static const Color Gray;
        static const Color DarkGray;
        static const Color Transparent;
    };

    constexpr SDL_Color Color::GetSDLColor() const
    {
        return SDL_Color
        {
            static_cast<Uint8>(255.f * std::clamp(R, 0.f, 1.f)),
            static_cast<Uint8>(255.f * std::clamp(G, 0.f, 1.f)),
            static_cast<Uint8>(255.f * std::clamp(B, 0.f, 1.f)),
            static_cast<Uint8>(255.f * std::clamp(A, 0.f, 1.f))
        };
    }

    // inline's are needed to prevent linker error(s)
    inline constexpr Color Color::White       = Color(1.f, 1.f, 1.f);
    inline constexpr Color Color::Black       = Color(0.f, 0.f, 0.f);
    inline constexpr Color Color::Red         = Color(1.f, 0.f, 0.f);
    inline constexpr Color Color::Green       = Color(0.f, 1.f, 0.f);
    inline constexpr Color Color::Blue        = Color(0.f, 0.f, 1.f);
    inline constexpr Color Color::Yellow      = Color(1.f, 1.f, 0.f);
    inline constexpr Color Color::Orange      = Color(1.f, 0.392f, 0.f);
    inline constexpr Color Color::Magenta     = Color(1.f, 0.f, 1.f);
    inline constexpr Color Color::Cyan        = Color(0.f, 1.f, 1.f);
    inline constexpr Color Color::Purple      = Color(0.392f, 0.165f, 0.624f);
    inline constexpr Color Color::Gray        = Color(0.753f, 0.753f, 0.753f);
    inline constexpr Color Color::DarkGray    = Color(0.212f, 0.231f, 0.247f);
    inline constexpr Color Color::Transparent = Color(0.f, 0.f, 0.f, 0.f);


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


