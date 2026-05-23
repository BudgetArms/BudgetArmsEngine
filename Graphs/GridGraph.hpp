#pragma once

#include <glm/glm.hpp>

#include "Core/Utils.hpp"
#include "Graphs/ConnectionCostCalculator.hpp"
#include "Graphs/Graph.hpp"


namespace bae::Graphs
{
    class GraphNode;

    struct GridPosition
    {
        int Column;
        int Row;
    };

    class GridGraph : public Graph
    {
    public:
        GridGraph(
            const glm::vec2& position,
            int nrColumns, int nrRows,
            const glm::ivec2& cellSize,
            bool bIsDirectionalGraph,
            bool bIsConnectedDiagonally,
            float costStraight                                        = 1.f,
            float costDiagonal                                        = 1.5,
            GraphNodeFactory* factory                                 = nullptr,
            std::unique_ptr<ConnectionCostCalculator> uCostCalculator = nullptr
        );

        ~GridGraph() override = default;


        virtual void Render() const;


        [[nodiscard]] bool IsWithinBounds(GridPosition position) const;

        void AddConnectionsToAdjacentCells(GridPosition position);
        void AddConnectionsToAdjacentCells(int idx);


        [[nodiscard]] int GetRows() const { return m_NrOfRows; }
        [[nodiscard]] int GetColumns() const { return m_NrOfColumns; }
        [[nodiscard]] glm::ivec2 GetCellSize() const { return m_CellSize; }

        [[nodiscard]] glm::vec2 GetPosition(GridPosition position) const;
        [[nodiscard]] glm::vec2 GetOffsetPosition() const { return m_Position; }

        [[nodiscard]] int GetNodeId(GridPosition position) const;


        // Function overloading
        using Graph::GetNode;
        [[nodiscard]] GraphNode* GetNode(GridPosition position) const;

        [[nodiscard]] int GetNodeIdAtPosition(const glm::vec2& pos) const override;
        [[nodiscard]] GraphNode* GetNodeAtPosition(const glm::vec2& pos) const override;

        [[nodiscard]] glm::vec2 GetNodePos(int nodeId) const override;

        [[nodiscard]] GridPosition GetGridPosition(int idx) const;
        // Can return GridPosition outside grid
        [[nodiscard]] GridPosition GetGridPosition(const glm::vec2& position) const;


        bool m_bRenderNodes{ false };
        bool m_bRenderConnections{ false };

        Utils::Color m_RenderColorNode{ Utils::Color::Gray, 0.5f };
        Utils::Color m_RenderColorNodeOutlines{ Utils::Color::Red };
        Utils::Color m_RenderColorConnections{ Utils::Color::Cyan };

    private:
        void InitializeGrid();
        void AddConnectionsInDirections(int idx, GridPosition position, const std::vector<glm::vec2>& directions);

        [[nodiscard]] virtual float CalculateConnectionCost(int fromIdx, int toIdx) const;


        int m_NrOfColumns;
        int m_NrOfRows;
        glm::ivec2 m_CellSize;

        bool m_IsConnectedDiagonally;
        float m_DefaultCostStraight;
        float m_DefaultCostDiagonal;

        const std::vector<glm::vec2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
        const std::vector<glm::vec2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

        const float m_MaximumConnectionCost{ 100000.f };
        const glm::vec2 m_Position;

        std::unique_ptr<ConnectionCostCalculator> m_uCostCalculator;
    };
}


