#pragma once

#include "Graphs/GraphNode.h"


namespace bae::Graphs
{
	class ConnectionCostCalculator
	{
	public:
		ConnectionCostCalculator() = default;
		virtual ~ConnectionCostCalculator() = default;


		virtual float CalculateConnectionCost(const Graph* const pGraph, int fromNodeId, int toNodeId) const = 0;


	};
}

