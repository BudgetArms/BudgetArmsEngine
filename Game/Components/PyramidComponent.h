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

        int GetCellColor(int row, int col) const;
        void SetCellColor(int row, int col, int colorIndex);

        bool IsValidCell(int row, int col) const;

        int GetLevel() const { return m_Level; }
        void SetLevel(int level) { m_Level = level; UpdateColorScheme(); }

        // Get world position of a specific cell
        glm::vec2 GetCellWorldPosition(int row, int col) const;

        // Get cell from world position (for collision detection)
        bool GetCellFromWorldPosition(int worldX, int worldY, int& outRow, int& outCol) const;

        // Check if all cells are colored correctly for level completion
        bool IsLevelComplete() const;

        // Reset pyramid for new level
        void ResetPyramid();

    private:
        void InitializePyramid();
        void UpdateColorScheme();
        void CalculateCellPositions();
        int GetCellsInRow(int row) const;
        int GetRowStartColumn(int row) const;


        static const int PYRAMID_ROWS = 7;          // Fixed pyramid size
        static const int PYRAMID_COLUMNS = 7;       // Maximum columns (top row has 1, bottom has 7)
        static const int CELL_WIDTH = 90;           // Width of each pyramid cell
        //static const int CELL_HEIGHT = 32;          // Height of each pyramid cell
        static const int CELL_HEIGHT = 90;          // Height of each pyramid cell
        static const int PYRAMID_OFFSET_X = 360;    // Center offset for pyramid
        static const int PYRAMID_OFFSET_Y = 100;    // Top offset for pyramid


        struct PyramidCell
        {
            int currentColor;    // Current color index (0 = base, 1+ = level colors)
            int targetColor;     // Target color for this level
            bool isActive;       // Whether this cell exists in the pyramid
            glm::vec2 position;  // World position of this cell
        };

        bae::SpriteComponent* m_SpriteComp;
        std::unique_ptr<bae::Texture2D> m_Texture;
        std::vector<std::vector<PyramidCell>> m_Cells;

        int m_Level;
        int m_BaseColorIndex;       // Base color for current level
        int m_TargetColorIndex;     // Target color for current level



    };
}

