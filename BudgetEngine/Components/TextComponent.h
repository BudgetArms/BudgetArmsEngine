#pragma once
#include <string>
#include <memory>
#include "Core/GameObject.h"
#include "Components/TransformComponent.h"

namespace bae
{
    class Font;
    class Texture2D;
    class TextComponent : public Component
    {
    public:
        TextComponent(GameObject& owner, const std::string& text, std::shared_ptr<Font> font = nullptr);
        virtual ~TextComponent() = default;

        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) = delete;


        virtual void Update() override;
        virtual void Render() const override;

        void SetText(const std::string& text);
        Font* GetFont() const { return m_Font.get(); };
        void SetFont(std::shared_ptr<Font> font);


    private:
        bool m_NeedsUpdate;
        std::string m_Text;
        std::shared_ptr<Font> m_Font;


    protected:
        std::shared_ptr<Texture2D> m_TextTexture;


    };
}


