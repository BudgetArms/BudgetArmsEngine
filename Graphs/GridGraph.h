#pragma once

#include <glm.hpp>

#include "Core/Utils.h"
#include "Graphs/Graph.h"
#include "Graphs/ConnectionCostCalculator.h"


namespace bae::Graphs
{
	class GraphNode;
	//class ConnectionCostCalculator;

	class GridGraph : public Graph
	{
	public:

		GridGraph(
			const glm::vec2& position,
			int columns, int rows, const glm::ivec2& cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f,
			float costDiagonal = 1.5, GraphNodeFactory* const factory = nullptr, std::unique_ptr<ConnectionCostCalculator> uCostCalculator = nullptr
		);
		virtual ~GridGraph();


		virtual void Render() const;


		bool IsWithinBounds(int col, int row) const;

		void AddConnectionsToAdjacentCells(int col, int row) { AddConnectionsToAdjacentCells(GetNodeId(col, row)); }
		void AddConnectionsToAdjacentCells(int idx);


		int GetRows() const { return m_NrOfRows; }
		int GetColumns() const { return m_NrOfColumns; }
		glm::ivec2 GetCellSize() const { return m_CellSize; }

		glm::vec2 GetPosition(int col, int row) const;
		glm::vec2 GetOffsetPosition() const { return m_Position; }

		int GetNodeId(int col, int row) const { return row * m_NrOfColumns + col; }

		// Function overloading
		using Graph::GetNode;
		GraphNode* GetNode(int col, int row) const { return Graph::GetNode(GetNodeId(col, row)); }

		int GetNodeIdAtPosition(const glm::vec2& pos) const override;
		GraphNode* const GetNodeAtPosition(const glm::vec2& pos) const override;

		glm::vec2 GetNodePos(int nodeId) const override;
		std::pair<int, int> GetRowAndColumn(int idx) const { return { idx / m_NrOfColumns, idx % m_NrOfColumns }; }
		glm::vec2 GetRowAndColumn(const glm::vec2& pos);


		bool m_bRenderNodes{ false };
		bool m_bRenderConnections{ false };

		bae::Utils::Color m_RenderColorNode{ bae::Utils::Color::Gray, 0.5f };
		bae::Utils::Color m_RenderColorNodeOutlines{ bae::Utils::Color::Red };
		bae::Utils::Color m_RenderColorConnections{ bae::Utils::Color::Cyan };


	private:
		void InitializeGrid();
		void AddConnectionsInDirections(int idx, int col, int row, const std::vector<glm::vec2>& directions);

		virtual float CalculateConnectionCost(int fromIdx, int toIdx) const;


		int m_NrOfColumns;
		int m_NrOfRows;
		glm::ivec2 m_CellSize;

		bool m_IsConnectedDiagonally;
		float m_DefaultCostStraight;
		float m_DefaultCostDiagonal;

		const std::vector<glm::vec2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		const std::vector<glm::vec2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

		glm::vec2 m_Position;

		std::unique_ptr<ConnectionCostCalculator> m_uCostCalculator;


	};
}


