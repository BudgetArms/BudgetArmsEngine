#pragma once

#include "Components/Component.h"


namespace bae
{
    class GameObject;
    class TextureComponent : public Component
    {
    public:
        explicit TextureComponent(GameObject& owner, const std::string& filename);
        virtual ~TextureComponent() = default;

        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator=(const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;

        virtual void Render() const;

        void SetTexture(const std::string& filename);


    private:
        std::shared_ptr<Texture2D> m_Texture{};


    protected:


    };
}