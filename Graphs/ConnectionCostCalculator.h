#pragma once

#include "Graphs/Graph.h"
#include "Graphs/GraphNode.h"


namespace bae::Graphs
{
    class ConnectionCostCalculator
    {
    public:
        ConnectionCostCalculator()          = default;
        virtual ~ConnectionCostCalculator() = default;


        virtual float CalculateConnectionCost(const bae::Graphs::Graph* pGraph, int fromNodeId, int toNodeId) const = 0;
    };
}

