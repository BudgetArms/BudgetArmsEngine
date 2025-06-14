#include "PyramidComponent.h"

#include "Components/SpriteComponent.h" 
#include "Core/Renderer.h" 


using namespace Game;


// ai used

PyramidComponent::PyramidComponent(bae::GameObject& owner) :
    bae::Component(owner),
    m_SpriteComp{},
    m_Texture{},
    m_Cells{},
    m_Level{ 1 },
    m_BaseColorIndex{ 0 },
    m_TargetColorIndex{ 1 }
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, "Textures/60496.png", SDL_Rect(0, 160, 720, 192), 9, 54, 16);
    m_Texture = std::make_unique<bae::Texture2D>("Resources/Textures/60496.png");

    m_SpriteComp = m_Owner->GetComponent<bae::SpriteComponent>();
    if (!m_SpriteComp)
    {
        std::cout << "spritecomponent for some reason not loaded\n";
        return;
    }

    auto& location = m_Owner->GetWorldLocation();
    m_SpriteComp->SetDstRect(SDL_Rect(static_cast<int>(location.x), static_cast<int>(location.y), 0, 0));

    InitializePyramid();
    UpdateColorScheme();
}

void PyramidComponent::Update()
{
    if (m_SpriteComp)
        m_SpriteComp->Update();


}

void PyramidComponent::Render() const
{
    if (!m_SpriteComp)
        return;

    // works
    /*
    m_SpriteComp->m_Index = 12;
    auto dstRect = m_SpriteComp->GetDstRect();
    dstRect.x = 200;
    dstRect.y = 200;
    dstRect.w = CELL_WIDTH;
    dstRect.h = CELL_HEIGHT;

    auto srcRect = m_SpriteComp->GetSrcRect();
    srcRect.w = static_cast<int>(srcRect.w / 2.f);

    bae::Renderer::GetInstance().RenderTexture(*m_Texture, srcRect, dstRect);
    //*/

    //*

    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        for (int col = 0; col < GetCellsInRow(row); ++col)
        {
            const auto& cell = m_Cells[row][col];
            if (cell.isActive)
            {
                SDL_Rect dstRect =
                {
                    static_cast<int>(cell.position.x),
                    static_cast<int>(cell.position.y),
                    CELL_WIDTH,
                    CELL_HEIGHT
                };

                auto srcRect = m_SpriteComp->GetSrcRect();
                if (row != 0)
                {
                    srcRect.w = static_cast<int>(srcRect.w / 2.f);
                    srcRect.x += static_cast<int>(srcRect.w);
                }
                bae::Renderer::GetInstance().RenderTexture(*m_Texture, srcRect, dstRect);
            }
        }
    }

    //*/

}



void PyramidComponent::SetCellColor(int row, int col, int colorIndex)
{
    if (IsValidCell(row, col))
    {
        m_Cells[row][col].currentColor = colorIndex;
    }
}

int PyramidComponent::GetCellColor(int row, int col) const
{
    if (IsValidCell(row, col))
        return m_Cells[row][col].currentColor;

    return -1;
}

bool PyramidComponent::IsValidCell(int row, int col) const
{
    if (row < 0 || row >= PYRAMID_ROWS)
        return false;

    int cellsInRow = GetCellsInRow(row);
    return col >= 0 && col < cellsInRow && m_Cells[row][col].isActive;
}

glm::vec2 PyramidComponent::GetCellWorldPosition(int row, int col) const
{
    if (IsValidCell(row, col))
    {
        return m_Cells[row][col].position;
    }
    return { -1, -1 };
}

bool PyramidComponent::GetCellFromWorldPosition(int worldX, int worldY, int& outRow, int& outCol) const
{
    // Check each cell to see if the world position falls within it
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        for (int col = 0; col < GetCellsInRow(row); ++col)
        {
            if (m_Cells[row][col].isActive)
            {
                const auto& pos = m_Cells[row][col].position;
                if (worldX >= pos.x && worldX < pos.x + CELL_WIDTH &&
                    worldY >= pos.y && worldY < pos.y + CELL_HEIGHT)
                {
                    outRow = row;
                    outCol = col;
                    return true;
                }
            }
        }
    }
    return false;
}

bool PyramidComponent::IsLevelComplete() const
{
    // Check if all cells have the target color
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        for (int col = 0; col < GetCellsInRow(row); ++col)
        {
            if (m_Cells[row][col].isActive)
            {
                if (m_Cells[row][col].currentColor != m_Cells[row][col].targetColor)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void PyramidComponent::ResetPyramid()
{
    // Reset all cells to base color
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        for (int col = 0; col < GetCellsInRow(row); ++col)
        {
            if (m_Cells[row][col].isActive)
            {
                m_Cells[row][col].currentColor = m_BaseColorIndex;
            }
        }
    }
}

void PyramidComponent::InitializePyramid()
{
    // Initialize the 2D vector
    m_Cells.resize(PYRAMID_ROWS);
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        m_Cells[row].resize(PYRAMID_COLUMNS);
    }

    // Set up the pyramid structure (isometric pyramid shape)
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        int cellsInThisRow = GetCellsInRow(row);
        int startCol = GetRowStartColumn(row);

        for (int col = 0; col < PYRAMID_COLUMNS; ++col)
        {
            if (col >= startCol && col < startCol + cellsInThisRow)
            {
                m_Cells[row][col].isActive = true;
                m_Cells[row][col].currentColor = m_BaseColorIndex;
                m_Cells[row][col].targetColor = m_TargetColorIndex;
            }
            else
            {
                m_Cells[row][col].isActive = false;
            }
        }
    }

    CalculateCellPositions();
}

void PyramidComponent::UpdateColorScheme()
{
    // Set color scheme based on level
    // Level 1: Blue to Orange (indices 0 to 1)
    // Level 2: Orange to Green (indices 1 to 2)
    // Level 3: Green to Purple (indices 2 to 3)
    // etc.

    m_BaseColorIndex = (m_Level - 1) % 8;
    m_TargetColorIndex = m_Level % 8;

    // Update target colors for all active cells
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        for (int col = 0; col < GetCellsInRow(row); ++col)
        {
            if (m_Cells[row][col].isActive)
            {
                m_Cells[row][col].targetColor = m_TargetColorIndex;
            }
        }
    }
}

void PyramidComponent::CalculateCellPositions()
{
    // Calculate isometric positions for each cell
    for (int row = 0; row < PYRAMID_ROWS; ++row)
    {
        int cellsInThisRow = GetCellsInRow(row);
        int startCol = GetRowStartColumn(row);

        for (int col = 0; col < cellsInThisRow; ++col)
        {
            if (m_Cells[row][startCol + col].isActive)
            {
                // Isometric positioning
                // Each row is offset downward and inward
                int x = PYRAMID_OFFSET_X + static_cast<int>((col - cellsInThisRow / 2.0f) * CELL_WIDTH);
                int y = PYRAMID_OFFSET_Y + row * (CELL_HEIGHT + 10); // Small gap between rows

                m_Cells[row][startCol + col].position = { x, y };
            }
        }
    }
}

int PyramidComponent::GetCellsInRow(int row) const
{
    // Pyramid structure: 
    // Row 0: 1 cell
    // Row 1: 2 cells  
    // Row 2: 3 cells
    // ...
    // Row 6: 7 cells
    return row + 1;
}

int PyramidComponent::GetRowStartColumn(int row) const
{
    // Center the cells in each row
    int cellsInRow = GetCellsInRow(row);
    return (PYRAMID_COLUMNS - cellsInRow) / 2;
}


