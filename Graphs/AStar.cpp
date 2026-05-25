#include "AStar.hpp"

#include <algorithm>
#include <list>

#include "Graphs/Graph.hpp"
#include "Graphs/Connection.hpp"
#include "Graphs/Node.hpp"


using namespace bae::Graphs;


AStar::AStar(Graph* graph, const Heuristic hFunction)
    : m_Graph(graph), m_HeuristicFunction(hFunction)
{
}


std::vector<Node*> AStar::FindPath(Node* const startNode, const Node* const destinationNode) const
{
    if(!startNode->IsValid() || !destinationNode->IsValid())
    {
        return std::vector<Node*>{};
    }

    if(startNode == destinationNode || startNode->m_Id == destinationNode->m_Id)
    {
        return std::vector<Node*>{};
    }


    std::list<NodeRecord> toBeCheckedNodeRecords{};
    std::vector<NodeRecord> checkedNodeRecords{};
    NodeRecord currentNodeRecord{};

    const NodeRecord startRecord
    {
        .CurrentNode        = startNode,
        .CurrentConnection  = nullptr,
        .CostSoFar          = 0.0f,
        .EstimatedTotalCost = 0.0f + GetHeuristicCost(startNode, destinationNode),
    };
    toBeCheckedNodeRecords.push_back(startRecord);

    while(!toBeCheckedNodeRecords.empty())
    {
        // std::ranges::min_element doesn't work due to NodeRecord not having
        // std::ranges::less implemented
        auto currentNodeRecordIt = std::min_element(toBeCheckedNodeRecords.begin(), toBeCheckedNodeRecords.end());
        currentNodeRecord        = *currentNodeRecordIt;

        // if CurrentNode is Goal, then exit while loop
        if(currentNodeRecord.CurrentNode == destinationNode)
        {
            break;
        }

        for(Connection* currentConnection : m_Graph->FindConnectionsFrom(currentNodeRecord.CurrentNode->m_Id))
        {
            // Aka neighbor node
            Node* const nextNode = m_Graph->GetNode(currentConnection->GetToNodeId()).get();

            const float currentGCost = currentNodeRecord.CostSoFar + currentConnection->m_Weight;

            // Check if NextNode has already been checked
            const auto foundCheckedRecordIt = std::ranges::find_if(checkedNodeRecords,
                                                                   [nextNode](const NodeRecord& checkedNodeRecord)
                                                                   {
                                                                       if(checkedNodeRecord.CurrentNode == nextNode)
                                                                       {
                                                                           return true;
                                                                       }

                                                                       return false;
                                                                   });

            const bool hasNodeAlreadyBeenChecked = (foundCheckedRecordIt != checkedNodeRecords.end());
            if(hasNodeAlreadyBeenChecked)
            {
                // if an already existing connection to the same node is cheaper, skip this node
                // else, remove the existing connection, since it's bad
                if(foundCheckedRecordIt->CostSoFar < currentGCost)
                {
                    continue;
                }
                else
                {
                    checkedNodeRecords.erase(foundCheckedRecordIt);
                }
            }


            // Check if NextNode is in the toBeCheck list
            const auto foundToBeCheckedRecordIt = std::ranges::find_if(toBeCheckedNodeRecords,
                                                                       [nextNode](
                                                                   const NodeRecord& toBeCheckedNodeRecord)
                                                                       {
                                                                           if(toBeCheckedNodeRecord.CurrentNode ==
                                                                               nextNode)
                                                                           {
                                                                               return true;
                                                                           }

                                                                           return false;
                                                                       });

            const bool bIsNodeInToBeCheckedRecords = (foundToBeCheckedRecordIt != toBeCheckedNodeRecords.end());
            if(bIsNodeInToBeCheckedRecords)
            {
                if(foundToBeCheckedRecordIt->CostSoFar < currentGCost)
                {
                    continue;
                }
                else
                {
                    toBeCheckedNodeRecords.erase(foundToBeCheckedRecordIt);
                }
            }

            NodeRecord connectionRecord
            {
                .CurrentNode        = nextNode,
                .CurrentConnection  = currentConnection,
                .CostSoFar          = currentGCost,
                .EstimatedTotalCost = currentGCost + GetHeuristicCost(nextNode, destinationNode),
            };

            toBeCheckedNodeRecords.push_back(connectionRecord);
        }

        if(currentNodeRecordIt == toBeCheckedNodeRecords.end())
        {
            break;
        }

        toBeCheckedNodeRecords.erase(currentNodeRecordIt);
        checkedNodeRecords.push_back(currentNodeRecord);
    }

    if(checkedNodeRecords.empty())
    {
        return std::vector<Node*>{};
    }


    // Backtracking
    std::vector<Node*> path{};

    while(currentNodeRecord.CurrentNode != startNode)
    {
        if(currentNodeRecord.CurrentNode == nullptr)
        {
            return std::vector<Node*>{};
        }

        path.push_back(currentNodeRecord.CurrentNode);

        const int previousNodeId        = currentNodeRecord.CurrentConnection->GetFromNodeId();
        const auto previousNodeRecordIt = std::ranges::find_if(checkedNodeRecords,
                                                               [previousNodeId](const NodeRecord& CheckedNodeRecord)
                                                               {
                                                                   if(CheckedNodeRecord.CurrentNode->m_Id ==
                                                                       previousNodeId)
                                                                   {
                                                                       return true;
                                                                   }

                                                                   return false;
                                                               });

        const bool hasFoundPreviousRecord = previousNodeRecordIt != checkedNodeRecords.end();
        if(!hasFoundPreviousRecord)
        {
            // TODO: Change back
            path.push_back(startNode);
            return path;
            // return std::vector<Node*>{};
        }

        currentNodeRecord = *previousNodeRecordIt;
    }

    path.push_back(startNode);
    std::ranges::reverse(path);

    return path;
}

float AStar::GetHeuristicCost(const Node* const pStartNode, const Node* const pEndNode) const
{
    const glm::vec2 toDestination = m_Graph->GetNode(pEndNode->m_Id)->m_Position
            - m_Graph->GetNode(pStartNode->m_Id)->m_Position;

    return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
}


