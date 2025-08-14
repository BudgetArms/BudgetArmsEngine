#pragma once

#include <vector>

#include "Graphs/Heuristic.h"


namespace bae::Graphs
{
	class Graph;
	class GraphNode;
	class GraphConnection;
	class AStar
	{
	public:
		AStar(Graph* const pGraph, bae::Graphs::Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord final
		{
			GraphNode* pNode = nullptr;
			GraphConnection* pConnection = nullptr;
			float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<GraphNode*> FindPath(GraphNode* const pStartNode, GraphNode* const pDestinationNode);


	private:
		float GetHeuristicCost(GraphNode* const pStartNode, GraphNode* const pEndNode) const;


		Graph* m_pGraph;
		bae::Graphs::Heuristic m_HeuristicFunction;


	};
}


