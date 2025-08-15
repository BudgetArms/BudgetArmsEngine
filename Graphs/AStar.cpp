#include "AStar.h"

#include <list>

#include "Graphs/Graph.h"
#include "Graphs/GraphNode.h"
#include "Graphs/GraphConnection.h"


using namespace bae::Graphs;


AStar::AStar(Graph* const pGraph, bae::Graphs::Heuristic hFunction)
	: m_pGraph(pGraph)
	, m_HeuristicFunction(hFunction)
{
}


std::vector<GraphNode*>AStar::FindPath(GraphNode* const pStartNode, GraphNode* const pGoalNode)
{
	std::vector<GraphNode*> path{};
	std::list<NodeRecord> openList;   // nodes that need to be checked, not a queue anymore bc cost
	std::list<NodeRecord> closedList; // nodes that are already checked
	NodeRecord currentNodeRecord{};   // node that will be checked nextto be evaluated node


	openList.emplace_back(
		NodeRecord
		{
			.pNode = pStartNode,
			.pConnection = nullptr,
			.estimatedTotalCost = GetHeuristicCost(pStartNode,pGoalNode)
		});


	while (!openList.empty())
	{
		// std::ranges::min_element does not work for std::list, strange
		auto currentNodeRecordIt = std::min_element(openList.begin(), openList.end());
		currentNodeRecord = *currentNodeRecordIt;

		if (currentNodeRecord.pNode == pGoalNode)
			break;


		for (auto& nextNodeConnection : m_pGraph->GetConnectionsFromNode(currentNodeRecord.pNode))
		{
			auto pNextNode = m_pGraph->GetNode(nextNodeConnection->GetToNodeId());

			float nextNodeGCost = currentNodeRecord.costSoFar + nextNodeConnection->GetCost();

			// Check if the next node is already checked (eg. In closedList)
			auto closedNodeRecordIt = std::ranges::find_if(closedList,
				[&pNextNode](NodeRecord& checkedRecord)
				{
					return checkedRecord.pNode == pNextNode;
				});

			// if this next Connection path is cheaper, remove the old Record
			// else skip this connection and go the next one, bc it's slow anyway
			if (closedNodeRecordIt != closedList.end())
			{
				if (nextNodeGCost < closedNodeRecordIt->costSoFar)
					closedList.remove(*closedNodeRecordIt);
				else
					continue;

			}


			// same for closedNode, but now for the nodes that we are going to check next
			auto openNodeRecordIt = std::ranges::find_if(openList,
				[&pNextNode](NodeRecord& openListRecord)
				{
					return openListRecord.pNode == pNextNode;
				});

			if (openNodeRecordIt != openList.end())
			{
				if (nextNodeGCost < openNodeRecordIt->costSoFar)
					openList.remove(*openNodeRecordIt);
				else
					continue;
			}


			// This point is only reached if
			// NextConnectionNode was not in openList (to check next) and closedList (already checked nodes)
			// or if the NextConnectionNode's cost was lower than the already exiting data saved

			NodeRecord nextNodeRecord
			{
				.pNode = pNextNode,
				.pConnection = nextNodeConnection.get(),
				.costSoFar = nextNodeGCost,
				.estimatedTotalCost = nextNodeGCost + GetHeuristicCost(pNextNode, pGoalNode),
			};


			openList.push_back(nextNodeRecord);

		}

		// We went through all the connections, NEXT!
		openList.remove(currentNodeRecord);
		closedList.push_back(currentNodeRecord);
	}



	// Backtracking
	if (currentNodeRecord.pNode != pGoalNode)
		return path;



	for (GraphNode* previousNode = nullptr; currentNodeRecord.pNode != pStartNode; )
	{
		path.push_back(currentNodeRecord.pNode);

		if (currentNodeRecord.pConnection)
			previousNode = m_pGraph->GetNode(currentNodeRecord.pConnection->GetFromNodeId());
		else
			break;

		auto previousNodeRecordIt = std::ranges::find_if(closedList,
			[&previousNode](NodeRecord& closedNodeRecord)
			{
				return closedNodeRecord.pNode == previousNode;
			});

		if (previousNodeRecordIt != closedList.end())
			currentNodeRecord = *previousNodeRecordIt;

	}


	path.push_back(pStartNode);
	std::ranges::reverse(path);

	return path;
}

float AStar::GetHeuristicCost(GraphNode* const pStartNode, GraphNode* const pEndNode) const
{
	glm::vec2 toDestination = m_pGraph->GetNodePos(pEndNode->GetId()) - m_pGraph->GetNodePos(pStartNode->GetId());
	return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
}


