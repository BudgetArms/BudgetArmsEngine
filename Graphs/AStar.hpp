#pragma once

#include <vector>

#include "Graphs/Heuristic.hpp"
#include "Graphs/HeuristicFunctions.hpp"


namespace bae::Graphs
{
    class Graph;
    class Node;
    class Connection;

    class AStar
    {
    public:
        AStar(Graph* graph, Heuristic hFunction);

        // stores the optimal connection to a node and its total costs related to the start and end node of the path
        struct NodeRecord final
        {
            Node* CurrentNode             = nullptr;
            Connection* CurrentConnection = nullptr;
            float CostSoFar               = 0.f; // accumulated g-costs of all the connections leading up to this one
            float EstimatedTotalCost      = 0.f; // f-cost (= costSoFar + h-cost)

            bool operator==(const NodeRecord& other) const
            {
                return CurrentNode == other.CurrentNode
                        && CurrentConnection == other.CurrentConnection
                        && CostSoFar == other.CostSoFar
                        && EstimatedTotalCost == other.EstimatedTotalCost;
            }

            bool operator<(const NodeRecord& other) const
            {
                return EstimatedTotalCost < other.EstimatedTotalCost;
            }
        };


        [[nodiscard]] std::vector<Node*> FindPath(Node* startNode, const Node* destinationNode) const;

    private:
        [[nodiscard]] float GetHeuristicCost(const Node* pStartNode, const Node* pEndNode) const;


        Graph* m_Graph;
        Heuristic m_HeuristicFunction;
    };
}


