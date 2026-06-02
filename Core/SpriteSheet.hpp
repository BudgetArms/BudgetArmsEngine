#pragma once

#include <filesystem>
#include <SDL3/SDL_rect.h>

#include "Wrappers/Texture2D.hpp"


namespace bae
{
    class SpriteSheet final
    {
    public:
        SpriteSheet(const std::filesystem::path& file, const SDL_FRect& srcRect,
                    int nrColumns, int nrSprites, const glm::ivec2& srcOffset = {});

        void Render() const;


        [[nodiscard]] SDL_FRect GetCurrentSpriteRect() const;


        void PreviousSprite();
        void NextSprite();

        int m_Index{};

        bool m_bUseParentLocation{ true };
        bool m_bUseParentRotation{ true };
        bool m_bUseParentScale{ true };

        bool m_bIsCenteredAtPosition{ true };

        SDL_FRect m_SrcRect;
        SDL_FRect m_DstRect{};

        glm::vec2 m_Position{};
        float m_Rotation{};
        glm::vec2 m_Scale{ 1, 1 };

    private:
        std::shared_ptr<Texture2D> m_Texture{};

        glm::vec2 m_SrcOffset{};


        const int m_NrSprites;
        const int m_NrColumns;
        const int m_NrRows;
    };
}
