#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

#include "Core/Utils.hpp"
#include "Wrappers/Font.hpp"
#include "Wrappers/Texture2D.hpp"


namespace bae
{
    class Text2D final
    {
    public:
        explicit Text2D(const std::string& text, std::shared_ptr<Font> font = nullptr,
                        const Utils::Color& color                           = Utils::Color::White);
        ~Text2D() = default;

        Text2D(const Text2D&)            = delete;
        Text2D(Text2D&&)                 = delete;
        Text2D& operator=(const Text2D&) = delete;
        Text2D& operator=(Text2D&&)      = delete;


        void Update();
        void Render() const;

        [[nodiscard]] std::string GetText();
        void SetText(const std::string& text);

        [[nodiscard]] Utils::Color GetColor();
        void SetColor(const Utils::Color& color);

        [[nodiscard]] Font* GetFont() const { return m_uFont.get(); }
        void SetFont(std::shared_ptr<Font> font);


        bool m_bIsCenteredAtPosition{ false };
        glm::vec2 m_Position{};
        float m_Rotation{};
        glm::vec2 m_Scale{ 1.f, 1.f };

    private:
        std::shared_ptr<Font> m_uFont;
        std::unique_ptr<Texture2D> m_uTextTexture;

        bool m_bNeedsUpdate{ true };
        std::string m_Text;
        Utils::Color m_Color;
    };
}


