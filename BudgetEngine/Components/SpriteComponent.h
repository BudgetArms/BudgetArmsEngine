#pragma once

#include "Components/TextureComponent.h"
#include <SDL.h>


namespace bae
{
    class GameObject;
    class SpriteComponent : public TextureComponent
    {
    public:
        // we should use a settings's struct to initialize the variables first, to make it more organized 
        explicit SpriteComponent(GameObject& owner, const std::string& filename, const SDL_Rect& srcRect,
            int nrColumns, int nrSprites, const glm::ivec2& srcOffset = {});
        virtual ~SpriteComponent() = default;

        SpriteComponent(const SpriteComponent& other) = delete;
        SpriteComponent(SpriteComponent&& other) = delete;
        SpriteComponent& operator=(const SpriteComponent& other) = delete;
        SpriteComponent& operator=(SpriteComponent&& other) = delete;

        virtual void Render() const override;


        SDL_Rect GetDstRect() const { return m_DstRect; };
        void SetDstRect(const SDL_Rect& dstRect) { m_DstRect = dstRect; };

        SDL_Rect GetCurrentSpriteRect() const;

        SDL_Rect GetSrcRect() const { return m_SrcRect; };
        SDL_Rect SetSrcRect(const SDL_Rect& srcRect) { m_SrcRect = srcRect; };

        void PreviousSprite();
        void NextSprite();


        int m_Index{};

        glm::ivec2 m_SrcOffset{};
        glm::ivec2 m_PositionOffset{};

        bool m_bUseParentLocation{ true };
        bool m_bUseParentRotation{ true };
        bool m_bUseParentScale{ true };


    private:
        SDL_Rect m_SrcRect;
        SDL_Rect m_DstRect;

        const int m_NrSprites;
        const int m_NrColumns;
        const int m_NrRows;


    };
}

