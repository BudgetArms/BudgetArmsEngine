#include "GridGraph.h"

#include "Core/Utils.h"
#include "Graphs/Graph.h"
#include "Graphs/GraphEnums.h"
#include "Graphs/GraphConnection.h"
#include "Graphs/ConnectionCostCalculator.h"


using namespace bae::Graphs;
namespace bu = bae::Utils;


GridGraph::GridGraph(
	const glm::vec2& position,
	int columns, int rows, const glm::ivec2& cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight,
	float costDiagonal, GraphNodeFactory* const pFactory, ConnectionCostCalculator* const pCostCalculator) :

	Graph(isDirectionalGraph, pFactory),
	m_NrOfColumns{ columns },
	m_NrOfRows{ rows },
	m_CellSize{ cellSize },
	m_IsConnectedDiagonally{ isConnectedDiagonally },
	m_DefaultCostStraight{ costStraight },
	m_DefaultCostDiagonal{ costDiagonal },
	m_Position{ position },
	m_pCostCalculator{ pCostCalculator }
{
	InitializeGrid();
}

GridGraph::~GridGraph()
{
	SAFE_DELETE(m_pCostCalculator);
}


void GridGraph::Render() const
{
	if (m_bRenderNodes)
	{
		for (int r = 0; r < m_NrOfRows; ++r)
		{
			for (int c = 0; c < m_NrOfColumns; ++c)
			{
				const int idx = GetNodeId(c, r);
				const glm::vec2 cellPos = GetNodePos(idx);

				const SDL_Rect nodeRect
				{
					static_cast<int>(cellPos.x - m_CellSize.x / 2.f),
					static_cast<int>(cellPos.y - m_CellSize.y / 2.f),
					m_CellSize.x, m_CellSize.y
				};

				bu::DrawRect(nodeRect, m_RenderColorNodeOutlines);

				const GraphNode* pGraphNode = GetNode(c, r);
				pGraphNode->Render();

			}
		}
	}

	if (m_bRenderConnections)
	{
		for (GraphNode* pNode : m_pActiveNodes)
		{
			for (GraphConnection* con : GetConnectionsFromNode(pNode->GetId()))
			{
				glm::vec2 startPos = GetNodePos(con->GetToNodeId());
				glm::vec2 endPos = GetNodePos(con->GetFromNodeId());

				bu::DrawLine(startPos, endPos, 1, m_RenderColorConnections);
			}
		}
	}

}


bool GridGraph::IsWithinBounds(int col, int row) const
{
	return (col >= 0 && col < m_NrOfColumns && row >= 0 && row < m_NrOfRows);
}

void GridGraph::AddConnectionsToAdjacentCells(int idx)
{
	auto [row, col] = GetRowAndColumn(idx);

	// Add connections in all directions, taking into account the dimensions of the grid
	AddConnectionsInDirections(idx, col, row, m_StraightDirections);

	if (m_IsConnectedDiagonally)
		AddConnectionsInDirections(idx, col, row, m_DiagonalDirections);

	OnGraphModified(false, true);
}

int GridGraph::GetNodeIdAtPosition(const glm::vec2& pos) const
{
	const int idx = InvalidNodeID;

	if (pos.x < 0 || pos.y < 0)
		return idx;

	int c = static_cast<int>((pos.x - m_Position.x) / m_CellSize.x);
	int r = static_cast<int>((pos.y - m_Position.y) / m_CellSize.y);

	if (!IsWithinBounds(c, r))
		return idx;

	return GetNodeId(c, r);
}

GraphNode* const GridGraph::GetNodeAtPosition(const glm::vec2& pos) const
{
	return Graph::GetNode(GetNodeIdAtPosition(pos));
}

glm::vec2 GridGraph::GetNodePos(int nodeId) const
{
	auto [row, col] = GetRowAndColumn(nodeId);
	return CalculatePosition(col, row);
}

void GridGraph::InitializeGrid()
{
	// Create all nodes
	for (int r = 0; r < m_NrOfRows; ++r)
	{
		for (int c = 0; c < m_NrOfColumns; ++c)
		{
			const int idx = GetNodeId(c, r);
			AddNodeAtIndex(CreateNode(GetNodePos(idx)));
		}
	}

	// Create connections in each valid direction on each node
	for (int r = 0; r < m_NrOfRows; ++r)
		for (int c = 0; c < m_NrOfColumns; ++c)
			AddConnectionsToAdjacentCells(c, r);

}

void GridGraph::AddConnectionsInDirections(int idx, int col, int row, const std::vector<glm::vec2>& directions)
{
	for (const glm::vec2& d : directions)
	{
		const int neighborCol = col + static_cast<int>(d.x);
		const int neighborRow = row + static_cast<int>(d.y);

		if (IsWithinBounds(neighborCol, neighborRow))
		{
			int neighborIdx = neighborRow * m_NrOfColumns + neighborCol;
			float connectionCost = CalculateConnectionCost(idx, neighborIdx);

			if (!Graph::ConnectionExists(idx, neighborIdx)
				&& connectionCost < 100000) //Extra check for different terrain types
				AddConnection(new GraphConnection(idx, neighborIdx, connectionCost));
		}
	}

}

float GridGraph::CalculateConnectionCost(int fromIdx, int toIdx) const
{
	float cost = m_DefaultCostStraight;

	const glm::vec2 fromPos = Graph::GetNode(fromIdx)->GetPosition();
	const glm::vec2 toPos = Graph::GetNode(toIdx)->GetPosition();

	if (static_cast<int>(fromPos.y) != static_cast<int>(toPos.y) &&
		static_cast<int>(fromPos.x) != static_cast<int>(toPos.x))
	{
		cost = m_DefaultCostDiagonal;
	}

	if (m_pCostCalculator != nullptr)
		cost *= m_pCostCalculator->CalculateConnectionCost(static_cast<const Graph*>(this), fromIdx, toIdx);

	return cost;
}

glm::vec2 GridGraph::CalculatePosition(int col, int row) const
{
	const glm::vec2 cellCenterOffset = { m_CellSize.x * .5f, m_CellSize.y * .5f };
	const glm::vec2 cellPosition = glm::vec2
	{
		static_cast<float>(col) * m_CellSize.x,
		static_cast<float>(row) * m_CellSize.y
	};

	return m_Position + cellPosition + cellCenterOffset;
}


