#pragma once

#include <glm.hpp>

#include "Core/Utils.h"
#include "Graphs/Graph.h"
#include "Graphs/ConnectionCostCalculator.h"


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
			int nrColumns, int nrRows, const glm::ivec2& cellSize, bool isDirectionalGraph, bool isConnectedDiagonally, float costStraight = 1.f,
			float costDiagonal = 1.5, GraphNodeFactory* const factory = nullptr, std::unique_ptr<ConnectionCostCalculator> uCostCalculator = nullptr
		);

        ~GridGraph() override {};


		virtual void Render() const;


		bool IsWithinBounds(GridPosition position) const;

		void AddConnectionsToAdjacentCells(GridPosition position);
		void AddConnectionsToAdjacentCells(int idx);


		int GetRows() const { return m_NrOfRows; }
		int GetColumns() const { return m_NrOfColumns; }
		glm::ivec2 GetCellSize() const { return m_CellSize; }

		glm::vec2 GetPosition(GridPosition position) const;
		glm::vec2 GetOffsetPosition() const { return m_Position; }

		int GetNodeId(GridPosition position) const;

		// Function overloading
		using Graph::GetNode;
		GraphNode* GetNode(GridPosition position) const;

		int GetNodeIdAtPosition(const glm::vec2& pos) const override;
		GraphNode* const GetNodeAtPosition(const glm::vec2& pos) const override;

		glm::vec2 GetNodePos(int nodeId) const override;
		GridPosition GetGridPosition(int idx) const;


		bool m_bRenderNodes{ false };
		bool m_bRenderConnections{ false };

		bae::Utils::Color m_RenderColorNode{ bae::Utils::Color::Gray, 0.5f };
		bae::Utils::Color m_RenderColorNodeOutlines{ bae::Utils::Color::Red };
		bae::Utils::Color m_RenderColorConnections{ bae::Utils::Color::Cyan };


	private:
		void InitializeGrid();
		void AddConnectionsInDirections(int idx, GridPosition position, const std::vector<glm::vec2>& directions);

		virtual float CalculateConnectionCost(int fromIdx, int toIdx) const;


		int m_NrOfColumns;
		int m_NrOfRows;
		glm::ivec2 m_CellSize;

		bool m_IsConnectedDiagonally;
		float m_DefaultCostStraight;
		float m_DefaultCostDiagonal;

		const std::vector<glm::vec2> m_StraightDirections = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		const std::vector<glm::vec2> m_DiagonalDirections = { { 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

	    const float m_MaximumConnectionCost{ 100000.f };
		glm::vec2 m_Position;

		std::unique_ptr<ConnectionCostCalculator> m_uCostCalculator;


	};
}


