#pragma once

#include <vector>
#include <memory>

#include "Components/Component.h" 
#include "Wrappers/Texture2D.h" 


// ai used
namespace bae
{
    class SpriteComponent;
}

namespace Game
{

    class PyramidComponent : public bae::Component
    {
    public:
        PyramidComponent(bae::GameObject& owner);
        ~PyramidComponent() = default;


        virtual void Update() override;
        virtual void Render() const override;

        void ResetPyramid();
        bool IsLevelComplete() const;

        int GetCellColorIndex(int row, int col) const;
        void SetCellColorIndex(int row, int col, int colorIndex);

        bool IsValidCell(int row, int col) const;

        int GetLevel() const { return m_Level; }
        void SetLevel(int level);

        glm::vec2 GetCellPosition(int row, int col) const;
        bool GetCell(const glm::vec2& position, glm::ivec2& outCell) const;


    private:
        void InitializePyramid();
        void UpdateColorScheme();
        void CalculateCellPositions();

        struct PyramidCell
        {
            int currentColor;
            int targetColor;
            bool isActive;
            glm::vec2 position;
        };


        // the pyramid should also have some kind of state
        bae::SpriteComponent* m_SpriteComp;
        std::unique_ptr<bae::Texture2D> m_Texture;
        std::vector<std::vector<PyramidCell>> m_Cells;

        int m_Level;


    };
}


