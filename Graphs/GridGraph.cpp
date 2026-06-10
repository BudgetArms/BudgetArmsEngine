#include "GridGraph.hpp"

#include <iostream>

#include "Core/HelperFunctions.hpp"
#include "Core/Utils.hpp"
#include "Graphs/Connection.hpp"
#include "Graphs/Enums.hpp"
#include "Graphs/Graph.hpp"


using namespace bae::Graphs;
namespace bu = bae::Utils;


GridGraph::GridGraph(
    const glm::vec2& position,
    const int nrColumns, const int nrRows,
    const glm::ivec2& cellSize,
    const bool bIsDirectionalGraph,
    const bool bIsConnectedDiagonally,
    std::unique_ptr<NodeFactory> factory
) :
    Graph(bIsDirectionalGraph),
    m_NrOfColumns{ nrColumns },
    m_NrOfRows{ nrRows },
    m_CellSize{ cellSize },
    m_IsConnectedDiagonally{ bIsConnectedDiagonally },
    m_Position{ position },
    m_NodeFactory{ std::move(factory) }

{
    InitializeGrid();
}


void GridGraph::Render() const
{
    if(m_bRenderCells)
    {
        for(int row{}; row < m_NrOfRows; ++row)
        {
            for(int column{}; column < m_NrOfColumns; ++column)
            {
                const GridPosition gridPosition{ column, row };

                const int nodeId        = GetNodeId(gridPosition);
                const glm::vec2 cellPos = GetNodePos(nodeId);

                const SDL_FRect nodeRect
                {
                    cellPos.x - static_cast<float>(m_CellSize.x) / 2.f,
                    cellPos.y - static_cast<float>(m_CellSize.y) / 2.f,
                    static_cast<float>(m_CellSize.x),
                    static_cast<float>(m_CellSize.y)
                };

                bu::DrawRect(nodeRect, m_RenderColorNodeOutlines);
            }
        }
    }

    if(m_bRenderNodes)
    {
        for(int row{}; row < m_NrOfRows; ++row)
        {
            for(int column{}; column < m_NrOfColumns; ++column)
            {
                const GridPosition gridPosition{ column, row };
                const Node* node = GetNode(gridPosition);
                if(node->IsValid())
                {
                    node->Render();
                }
            }
        }
    }

    if(m_bRenderConnections)
    {
        for(const Node* pNode : GetActiveNodes())
        {
            for(const Connection* connection : FindConnectionsWith(pNode->m_Id))
            {
                if(!connection)
                {
                    continue;;
                }

                glm::vec2 startPos = GetNodePos(connection->GetToNodeId());
                glm::vec2 endPos   = GetNodePos(connection->GetFromNodeId());

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

void GridGraph::AddConnectionsToAdjacentCells(const int nodeId)
{
    if(!IsWithinBounds(GetGridPosition(nodeId)))
    {
        return;
    }

    const GridPosition position = GetGridPosition(nodeId);

    // Add connections in all directions, taking into account the dimensions of the grid
    AddConnectionsInDirections(nodeId, position, m_StraightDirections);

    if(m_IsConnectedDiagonally)
    {
        AddConnectionsInDirections(nodeId, position, m_DiagonalDirections);
    }
}


void GridGraph::AddConnectionsInDirections(const int nodeId, const GridPosition position,
                                           const std::vector<glm::vec2>& directions)
{
    if(!IsWithinBounds(GetGridPosition(nodeId)))
    {
        return;
    }

    for(const glm::vec2& direction : directions)
    {
        const GridPosition neighborGridPos =
        {
            position.Column + static_cast<int>(direction.x),
            position.Row + static_cast<int>(direction.y)
        };

        if(IsWithinBounds(neighborGridPos))
        {
            const int neighborNodeId = GetNodeId(neighborGridPos);
            AddConnection(std::make_unique<Connection>(nodeId, neighborNodeId));
        }
    }
}

void GridGraph::RemoveConnectionsInDirections(const int nodeId, const GridPosition position,
                                              const std::vector<glm::vec2>& directions)
{
    if(!IsWithinBounds(GetGridPosition(nodeId)))
    {
        return;
    }

    for(const glm::vec2& direction : directions)
    {
        const GridPosition neighborGridPos =
        {
            position.Column + static_cast<int>(direction.x),
            position.Row + static_cast<int>(direction.y)
        };

        if(!IsWithinBounds(neighborGridPos))
        {
            continue;
        }

        const int neighborNodeId = GetNodeId(neighborGridPos);
        if(!FindConnection(nodeId, neighborNodeId))
        {
            continue;
        }

        RemoveConnection(nodeId, neighborNodeId);
    }
}


glm::vec2 GridGraph::GetPosition(const GridPosition position) const
{
    const glm::vec2 cellPosition =
    {
        static_cast<float>(position.Column) * static_cast<float>(m_CellSize.x),
        static_cast<float>(position.Row) * static_cast<float>(m_CellSize.y)
    };

    return m_Position + cellPosition;
}

int GridGraph::GetNodeId(const GridPosition position) const
{
    return position.Row * m_NrOfColumns + position.Column;
}

Node* GridGraph::GetNode(const GridPosition position) const
{
    const int nodeId = GetNodeId(position);
    if(nodeId == InvalidNodeID)
    {
        return nullptr;
    }

    return Graph::GetNode(nodeId).get();
}

int GridGraph::GetNodeIdAtPosition(const glm::vec2& pos) const
{
    if(pos.x < 0 || pos.y < 0)
    {
        return InvalidNodeID;
    }

    const GridPosition gridPosition = GetGridPosition(pos);

    if(!IsWithinBounds(gridPosition))
    {
        return InvalidNodeID;
    }

    return GetNodeId(gridPosition);
}

Node* GridGraph::GetNodeAtPosition(const glm::vec2& pos) const
{
    return GetNode(GetGridPosition(pos));
}

glm::vec2 GridGraph::GetNodePos(const int nodeId) const
{
    return GetPosition(GetGridPosition(nodeId));
}

GridPosition GridGraph::GetGridPosition(const int nodeId) const
{
    return GridPosition
    {
        .Column = nodeId % m_NrOfColumns,
        .Row    = nodeId / m_NrOfColumns
    };
}


GridPosition GridGraph::GetGridPosition(const glm::vec2& position) const
{
    return GridPosition
    {
        .Column = static_cast<int>(std::floor((position.x - m_Position.x) / static_cast<float>(m_CellSize.x))),
        .Row    = static_cast<int>(std::floor((position.y - m_Position.y) / static_cast<float>(m_CellSize.y)))
    };
}

void GridGraph::InitializeGrid()
{
    m_Nodes.resize(m_NrOfRows * m_NrOfColumns);

    // Create all nodes
    for(int row{}; row < m_NrOfRows; ++row)
    {
        for(int column{}; column < m_NrOfColumns; ++column)
        {
            const int nodeId           = GetNodeId(GridPosition{ column, row });
            std::unique_ptr<Node> node = CreateNode(GetNodePos(nodeId));

            node->m_Id      = nodeId;
            m_Nodes[nodeId] = std::move(node);
        }
    }

    // Create connections in each valid direction on each node
    for(int row{}; row < m_NrOfRows; ++row)
    {
        for(int column{}; column < m_NrOfColumns; ++column)
        {
            AddConnectionsToAdjacentCells(GridPosition{ column, row });
        }
    }
}

std::unique_ptr<Node> GridGraph::CreateNode(const glm::vec2& position) const
{
    if(m_NodeFactory)
    {
        auto node = m_NodeFactory->CreateNode(position);
        if(!node)
        {
            std::cout << FUNCTION_NAME << " Failed to create Node" << '\n';
            return std::make_unique<Node>(position);
        }

        return node;
    }

    return std::make_unique<Node>(position);
}

