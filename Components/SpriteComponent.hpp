#pragma once

#include <SDL3/SDL.h>

#include "Components/TextureComponent.hpp"


namespace bae
{
    class GameObject;

    class SpriteComponent final : public TextureComponent
    {
    public:
        // we should use a setting's struct to initialize the variables first, to make it more organized
        explicit SpriteComponent(GameObject& owner, const std::string& filename, const SDL_FRect& srcRect,
                                 int nrColumns, int nrSprites, const glm::ivec2& srcOffset = {});


        void Render() const override;


        [[nodiscard]] SDL_FRect GetDstRect() const { return m_DstRect; }
        void SetDstRect(const SDL_FRect& dstRect) { m_DstRect = dstRect; }

        [[nodiscard]] SDL_FRect GetCurrentSpriteRect() const;

        [[nodiscard]] SDL_FRect GetSrcRect() const { return m_SrcRect; }
        void SetSrcRect(const SDL_FRect& srcRect) { m_SrcRect = srcRect; }

        void PreviousSprite();
        void NextSprite();

        void SetColumns(int nrColumns);
        void SetSprites(int nrSprites);

        int m_Index{};

        bool m_bUseParentLocation{ true };
        bool m_bUseParentRotation{ true };
        bool m_bUseParentScale{ true };

        bool m_bIsCenteredAtPosition{ true };

    private:
        glm::vec2 m_SrcOffset{};

        SDL_FRect m_SrcRect;
        SDL_FRect m_DstRect;

        int m_NrSprites;
        int m_NrColumns;
        int m_NrRows;
    };
}


