#include "GridGraph.hpp"

#include "Core/Utils.hpp"
#include "Graphs/ConnectionCostCalculator.hpp"
#include "Graphs/Graph.hpp"
#include "Graphs/GraphConnection.hpp"
#include "Graphs/GraphEnums.hpp"


using namespace bae::Graphs;
namespace bu = bae::Utils;


GridGraph::GridGraph(
    const glm::vec2& position,
    const int nrColumns, const int nrRows, const glm::ivec2& cellSize, const bool bIsDirectionalGraph,
    const bool bIsConnectedDiagonally,
    const float costStraight,
    const float costDiagonal, GraphNodeFactory* const factory,
    std::unique_ptr<ConnectionCostCalculator> uCostCalculator) :

    Graph(bIsDirectionalGraph, factory),
    m_NrOfColumns{ nrColumns },
    m_NrOfRows{ nrRows },
    m_CellSize{ cellSize },
    m_IsConnectedDiagonally{ bIsConnectedDiagonally },
    m_DefaultCostStraight{ costStraight },
    m_DefaultCostDiagonal{ costDiagonal },
    m_Position{ position },
    m_uCostCalculator{ std::move(uCostCalculator) }
{
    InitializeGrid();
}


void GridGraph::Render() const
{
    if(m_bRenderNodes)
    {
        for(int row = 0; row < m_NrOfRows; ++row)
        {
            for(int column = 0; column < m_NrOfColumns; ++column)
            {
                const GridPosition gridPosition{ column, row };

                const int idx           = GetNodeId(gridPosition);
                const glm::vec2 cellPos = GetNodePos(idx);

                const SDL_FRect nodeRect
                {
                    cellPos.x - static_cast<float>(m_CellSize.x) / 2.f,
                    cellPos.y - static_cast<float>(m_CellSize.y) / 2.f,
                    static_cast<float>(m_CellSize.x),
                    static_cast<float>(m_CellSize.y)
                };

                bu::DrawRect(nodeRect, m_RenderColorNodeOutlines);

                const GraphNode* pGraphNode = GetNode(gridPosition);
                pGraphNode->Render();
            }
        }
    }

    if(m_bRenderConnections)
    {
        for(const GraphNode* pNode : m_pActiveNodes)
        {
            for(const std::unique_ptr<GraphConnection>& uConnection : GetConnectionsFromNode(pNode->GetId()))
            {
                glm::vec2 startPos = GetNodePos(uConnection->GetToNodeId());
                glm::vec2 endPos   = GetNodePos(uConnection->GetFromNodeId());

                bu::DrawLine(startPos, endPos, 1, m_RenderColorConnections);
            }
        }
    }
}


bool GridGraph::IsWithinBounds(const GridPosition position) const
{
    if(position.Column < 0 || position.Row < 0)
    {
        return false;
    }

    if(position.Column >= m_NrOfColumns || position.Row >= m_NrOfRows)
    {
        return false;
    }

    return true;
}

void GridGraph::AddConnectionsToAdjacentCells(const GridPosition position)
{
    AddConnectionsToAdjacentCells(GetNodeId(position));
}

void GridGraph::AddConnectionsToAdjacentCells(const int idx)
{
    const GridPosition position = GetGridPosition(idx);

    // Add connections in all directions, taking into account the dimensions of the grid
    AddConnectionsInDirections(idx, position, m_StraightDirections);

    if(m_IsConnectedDiagonally)
    {
        AddConnectionsInDirections(idx, position, m_DiagonalDirections);
    }

    OnGraphModified(false, true);
}


void GridGraph::AddConnectionsInDirections(const int idx, const GridPosition position,
                                           const std::vector<glm::vec2>& directions)
{
    for(const glm::vec2& direction : directions)
    {
        const int neighborColumn = position.Column + static_cast<int>(direction.x);
        const int neighborRow    = position.Row + static_cast<int>(direction.y);

        if(IsWithinBounds(GridPosition{ neighborColumn, neighborRow }))
        {
            const int neighborIdx      = neighborRow * m_NrOfColumns + neighborColumn;
            const float connectionCost = CalculateConnectionCost(idx, neighborIdx);

            // Extra check for different terrain types
            if(!ConnectionExists(idx, neighborIdx) && connectionCost < m_MaximumConnectionCost)
            {
                AddConnection(std::make_unique<GraphConnection>(idx, neighborIdx, connectionCost));
            }
        }
    }
}

void GridGraph::RemoveConnectionsInDirections(const int idx, const GridPosition position,
                                              const std::vector<glm::vec2>& directions)
{
    for(const glm::vec2& direction : directions)
    {
        const int neighborColumn = position.Column + static_cast<int>(direction.x);
        const int neighborRow    = position.Row + static_cast<int>(direction.y);

        if(IsWithinBounds(GridPosition{ neighborColumn, neighborRow }))
        {
            const int neighborIdx = neighborRow * m_NrOfColumns + neighborColumn;

            if(!ConnectionExists(idx, neighborIdx))
            {
                continue;
            }

            RemoveConnection(idx, neighborIdx);
        }
    }
}


glm::vec2 GridGraph::GetPosition(const GridPosition position) const
{
    const glm::vec2 cellCenterOffset =
    {
        static_cast<float>(m_CellSize.x) / 2.f,
        static_cast<float>(m_CellSize.y) / 2.f
    };
    const glm::vec2 cellPosition =
    {
        static_cast<float>(position.Column) * static_cast<float>(m_CellSize.x),
        static_cast<float>(position.Row) * static_cast<float>(m_CellSize.y)
    };

    return m_Position + cellPosition + cellCenterOffset;
}

int GridGraph::GetNodeId(const GridPosition position) const
{
    return position.Row * m_NrOfColumns + position.Column;
}

GraphNode* GridGraph::GetNode(const GridPosition position) const
{
    return Graph::GetNode(GetNodeId(position));
}

int GridGraph::GetNodeIdAtPosition(const glm::vec2& pos) const
{
    if(pos.x < 0 || pos.y < 0)
    {
        return InvalidNodeID;
    }

    const GridPosition gridPosition
    {
        .Column = static_cast<int>((pos.x - m_Position.x) / static_cast<float>(m_CellSize.x)),
        .Row    = static_cast<int>((pos.y - m_Position.y) / static_cast<float>(m_CellSize.y))
    };

    if(!IsWithinBounds(gridPosition))
    {
        return InvalidNodeID;
    }

    return GetNodeId(gridPosition);
}

GraphNode* GridGraph::GetNodeAtPosition(const glm::vec2& pos) const
{
    return Graph::GetNode(GetNodeIdAtPosition(pos));
}

glm::vec2 GridGraph::GetNodePos(const int nodeId) const
{
    return GetPosition(GetGridPosition(nodeId));
}

GridPosition GridGraph::GetGridPosition(const int idx) const
{
    return GridPosition
    {
        .Column = idx % m_NrOfColumns,
        .Row    = idx / m_NrOfColumns
    };
}


GridPosition GridGraph::GetGridPosition(const glm::vec2& position) const
{
    return GridPosition
    {
        .Column = static_cast<int>((position.x - m_Position.x) / static_cast<float>(m_CellSize.x)),
        .Row    = static_cast<int>((position.y - m_Position.y) / static_cast<float>(m_CellSize.y))
    };
}

void GridGraph::InitializeGrid()
{
    // Create all nodes
    for(int row = 0; row < m_NrOfRows; ++row)
    {
        for(int column = 0; column < m_NrOfColumns; ++column)
        {
            const int idx = GetNodeId(GridPosition{ column, row });
            AddNodeAtIndex(CreateNode(GetNodePos(idx)));
        }
    }

    // Create connections in each valid direction on each node
    for(int row = 0; row < m_NrOfRows; ++row)
    {
        for(int column = 0; column < m_NrOfColumns; ++column)
        {
            AddConnectionsToAdjacentCells(GridPosition{ column, row });
        }
    }
}

float GridGraph::CalculateConnectionCost(const int fromIdx, const int toIdx) const
{
    float cost = m_DefaultCostStraight;

    const glm::vec2 fromPos = Graph::GetNode(fromIdx)->GetPosition();
    const glm::vec2 toPos   = Graph::GetNode(toIdx)->GetPosition();

    if(static_cast<int>(fromPos.y) != static_cast<int>(toPos.y) &&
        static_cast<int>(fromPos.x) != static_cast<int>(toPos.x))
    {
        cost = m_DefaultCostDiagonal;
    }

    if(m_uCostCalculator != nullptr)
    {
        cost *= m_uCostCalculator->CalculateConnectionCost(static_cast<const Graph*>(this), fromIdx, toIdx);
    }


    return cost;
}

