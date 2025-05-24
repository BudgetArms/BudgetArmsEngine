#pragma once

#include "Components/TextureComponent.h"
#include <SDL.h>


namespace bae
{
    class GameObject;
    class SpriteComponent : public TextureComponent
    {
    public:
        explicit SpriteComponent(GameObject& owner, const std::string& filename, const SDL_Rect& srcRect, int nrColumns, int nrSprites);
        virtual ~SpriteComponent() = default;

        SpriteComponent(const SpriteComponent& other) = delete;
        SpriteComponent(SpriteComponent&& other) = delete;
        SpriteComponent& operator=(const SpriteComponent& other) = delete;
        SpriteComponent& operator=(SpriteComponent&& other) = delete;

        virtual void Render() const override;

        //void SetR-()

        const SDL_Rect& GetDstRect() const { return m_DstRect; };                                      //
        void SetDstRect(const SDL_Rect& dstRect) { m_DstRect = dstRect; };                             //

        int GetCurrentIndex() const { return m_Index; };
        void SetCurrentIndex(int index) { m_Index = index; };

        void PreviousSprite();
        void NextSprite();


    private:
        SDL_Rect m_SrcRect;
        SDL_Rect m_DstRect;

        int m_Index{};

        const int m_NrSprites;
        const int m_NrColumns;
        const int m_NrRows;


    protected:


    };
}