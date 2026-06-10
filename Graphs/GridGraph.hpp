#pragma once

#include <glm/glm.hpp>

#include "Core/Utils.hpp"
#include "Graphs/Graph.hpp"
#include "Graphs/NodeFactory.hpp"


namespace bae::Graphs
{
    struct GridPosition
    {
        int Column;
        int Row;


        bool operator==(const GridPosition& other) const
        {
            return std::tie(Column, Row) == std::tie(other.Column, other.Row);
        }

        bool operator<(const GridPosition& other) const
        {
            return std::tie(Column, Row) < std::tie(other.Column, other.Row);
        }
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
            std::unique_ptr<NodeFactory> factory
        );

        ~GridGraph() override = default;


        virtual void Render() const;


        [[nodiscard]] bool IsWithinBounds(GridPosition position) const;

        void AddConnectionsToAdjacentCells(GridPosition position);
        void AddConnectionsToAdjacentCells(int nodeId);

        void AddConnectionsInDirections(int nodeId, GridPosition position, const std::vector<glm::vec2>& directions);
        void RemoveConnectionsInDirections(int nodeId, GridPosition position, const std::vector<glm::vec2>& directions);


        [[nodiscard]] int GetRows() const { return m_NrOfRows; }
        [[nodiscard]] int GetColumns() const { return m_NrOfColumns; }
        [[nodiscard]] glm::ivec2 GetCellSize() const { return m_CellSize; }

        [[nodiscard]] glm::vec2 GetPosition(GridPosition position) const;
        [[nodiscard]] glm::vec2 GetOffsetPosition() const { return m_Position; }

        [[nodiscard]] int GetNodeId(GridPosition position) const;


        [[nodiscard]] Node* GetNode(GridPosition position) const;


        [[nodiscard]] int GetNodeIdAtPosition(const glm::vec2& pos) const;
        [[nodiscard]] Node* GetNodeAtPosition(const glm::vec2& pos) const;

        [[nodiscard]] glm::vec2 GetNodePos(int nodeId) const;

        [[nodiscard]] GridPosition GetGridPosition(int nodeId) const;
        // Can return GridPosition outside grid
        [[nodiscard]] GridPosition GetGridPosition(const glm::vec2& position) const;


        bool m_bRenderCells{ false };
        bool m_bRenderNodes{ false };
        bool m_bRenderConnections{ false };

        Utils::Color m_RenderColorNode{ Utils::Color::Gray, 0.5f };
        Utils::Color m_RenderColorNodeOutlines{ Utils::Color::Red };
        Utils::Color m_RenderColorConnections{ Utils::Color::Cyan };

    private:
        void InitializeGrid();


        int m_NrOfColumns;
        int m_NrOfRows;
        glm::ivec2 m_CellSize;

        bool m_IsConnectedDiagonally;

        const std::vector<glm::vec2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
        const std::vector<glm::vec2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

        const glm::vec2 m_Position;

    protected:
        [[nodiscard]] std::unique_ptr<Node> CreateNode(const glm::vec2& position) const;


        std::unique_ptr<NodeFactory> m_NodeFactory;
    };
}


