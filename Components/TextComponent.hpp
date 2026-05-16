#pragma once

#include <memory>
#include <string>

#include "Component.hpp"
#include "Core/GameObject.hpp"
#include "Core/Utils.hpp"


namespace bae
{
    class Font;
    class Texture2D;

    class TextComponent : public Component
    {
    public:
        TextComponent(GameObject& owner, const std::string& text,
                      std::shared_ptr<Font> font = nullptr, const Utils::Color& color = Utils::Color::White);
        ~TextComponent() override;

        TextComponent(const TextComponent& other)            = delete;
        TextComponent(TextComponent&& other)                 = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other)      = delete;


        void Update() override;
        void Render() const override;

        std::string GetText();
        void SetText(const std::string& text);

        Utils::Color GetColor();
        void SetColor(const Utils::Color& color);

        [[nodiscard]] Font* GetFont() const { return m_Font.get(); }
        void SetFont(std::shared_ptr<Font> font);


        bool m_bIsCenteredAtPosition{ false };

    private:
        bool m_NeedsUpdate;
        std::string m_Text;
        Utils::Color m_Color;
        std::shared_ptr<Font> m_Font;

    protected:
        std::unique_ptr<Texture2D> m_TextTexture;
    };
}

