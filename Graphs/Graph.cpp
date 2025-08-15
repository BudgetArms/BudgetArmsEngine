#include "Graph.h"

#include <algorithm>

#include "Graphs/GraphNode.h"
#include "Graphs/GraphConnection.h"


using namespace bae::Graphs;


#pragma region Private


Graph::Graph(bool isDirectional, GraphNodeFactory* const pNodeFactory) :
	m_bIsDirectional{ isDirectional },
	m_NextNodeId{ 0 },
	m_uNodes{},
	m_uConnections{},
	m_sNodeFactory{ pNodeFactory }
{
}

// Copy constructor
Graph::Graph(const Graph& other) :
	m_uNodes{  },
	m_uConnections{ },
	m_AmountConnections{ other.m_AmountConnections },
	m_AmountNodes{ other.m_AmountNodes },
	m_NextNodeId{ other.m_NextNodeId },
	m_bIsDirectional{ other.m_bIsDirectional },
	m_sNodeFactory{ other.m_sNodeFactory }
{
	m_uConnections.clear();

	for (const std::unique_ptr<GraphNode>& uNode : other.m_uNodes)
	{
		if (uNode == nullptr)
			m_uNodes.push_back(nullptr);
		else
			m_uNodes.push_back(CloneNode(*uNode));

	}

	for (const std::vector<std::unique_ptr<GraphConnection>>& connectionList : other.m_uConnections)
	{
		//connectionList;

		std::vector<std::unique_ptr<GraphConnection>> newList;

		for (const std::unique_ptr<GraphConnection>& uConnection : connectionList)
			newList.push_back(std::make_unique<GraphConnection>(*uConnection));

		m_uConnections.push_back(std::move(newList));
	}


	UpdateNextNodeIndex();
	UpdateActiveNodes();
}


Graph::~Graph()
{
	Clear();
}

void Graph::Clear()
{
	m_uConnections.clear();
	m_uNodes.clear();
	m_NextNodeId = 0;
}


std::shared_ptr<Graph> Graph::Clone() const
{
	return std::shared_ptr<Graph>(new Graph(*this));
}



//== NODES == 
int Graph::AddNode(std::unique_ptr<GraphNode> pNode)
{
	pNode->SetId(m_NextNodeId);
	const int size = static_cast<int>(m_uNodes.size());
	if (size < m_NextNodeId + 1)
	{
		m_uNodes.resize(static_cast<size_t>(m_NextNodeId + 1));
		m_uConnections.resize(static_cast<size_t>(m_NextNodeId + 1));
	}

	const int nodeId = pNode->GetId();
	m_uNodes[nodeId] = std::move(pNode);

	++m_AmountNodes;

	UpdateNextNodeIndex();
	UpdateActiveNodes();

	return nodeId;
}


void Graph::RemoveNode(int index)
{
	if (!IsNodeValid(index))
		return;

	m_uNodes[index]->SetId(InvalidNodeID);
	m_uNodes[index].reset();
	--m_AmountNodes;


	if (!m_bIsDirectional)
	{
		//visit each neighbour and erase any connections leading to this pNode
		for (auto currentConnectionIt = m_uConnections[index].begin(); currentConnectionIt != m_uConnections[index].end();
			++currentConnectionIt)
		{
			for (auto currentEdgeOnToNode = m_uConnections[(*currentConnectionIt)->GetToNodeId()].begin();
				currentEdgeOnToNode != m_uConnections[(*currentConnectionIt)->GetToNodeId()].end();
				++currentEdgeOnToNode)
			{
				if ((*currentEdgeOnToNode)->GetToNodeId() == index)
				{
					std::unique_ptr<GraphConnection>& conPtr = *currentEdgeOnToNode;
					currentEdgeOnToNode = m_uConnections[(*currentConnectionIt)->GetToNodeId()].erase(currentEdgeOnToNode);
					--m_AmountConnections;
					conPtr.reset();

					break;
				}
			}
		}
	}

	//finally, clear this pNode's connections
	for (std::unique_ptr<GraphConnection>& pConnection : m_uConnections[index])
	{
		--m_AmountConnections;
		pConnection.reset();
	}

	m_uConnections[index].clear();

	UpdateNextNodeIndex();
	UpdateActiveNodes();
}


bool Graph::IsNodeValid(int index) const
{
	return (size_t)index < m_uNodes.size() && m_uNodes[index] != nullptr;
}

GraphNode* const Graph::GetNode(int index) const
{
	if (!IsNodeValid(index))
		return nullptr;

	return m_uNodes[index].get();
}

std::unique_ptr<GraphNode> Graph::GetNodeRef(int index)
{
	if (!IsNodeValid(index))
		return nullptr;

	return std::move(m_uNodes[index]);
}

int Graph::GetNodeIdAtPosition(const glm::vec2& pos, float errorMargin) const
{
	GraphNode* const pGraphNode = GetNodeAtPosition(pos, errorMargin);
	if (pGraphNode == nullptr)
		return InvalidNodeID;

	return pGraphNode->GetId();
}

GraphNode* const Graph::GetNodeAtPosition(const glm::vec2& position, float errorMargin) const
{
	const float nodeRadius = std::abs(m_DefaultNodeRadius * errorMargin);
	auto foundIt = std::ranges::find_if(m_pActiveNodes,
		[position, nodeRadius, this](GraphNode* pNode)
		{
			return pNode->GetId() != InvalidNodeID && (pNode->GetPosition() - position).length() < nodeRadius;
		});

	if (foundIt != m_pActiveNodes.end())
		return (*foundIt);
	else
		return nullptr;
}

glm::vec2 Graph::GetNodePos(int nodeId) const
{
	const auto pNode = GetNode(nodeId);
	if (pNode)
		return pNode->GetPosition();

	return glm::vec2{ 0, 0 };
}


//== CONNECTIONS ==
void Graph::AddConnection(std::unique_ptr<GraphConnection> pConnection)
{
	assert(IsNodeValid(pConnection->GetFromNodeId()) && IsNodeValid(pConnection->GetToNodeId()) && "<Graph::AddConnection>: invalid node index");

	const float cost = pConnection->GetCost();
	const int fromNodeId = pConnection->GetFromNodeId();
	const int toNodeId = pConnection->GetToNodeId();

	m_uConnections[pConnection->GetFromNodeId()].push_back(std::move(pConnection));
	++m_AmountConnections;

	if (!m_bIsDirectional)
	{
		auto oppositeConn = std::make_unique<GraphConnection>();
		oppositeConn->SetCost(cost);
		oppositeConn->SetFromNodeId(toNodeId);
		oppositeConn->SetToNodeId(fromNodeId);

		m_uConnections[toNodeId].push_back(std::move(oppositeConn));
		++m_AmountConnections;
	}
}

GraphConnection* const Graph::GetConnection(int fromNodeId, int toNodeId) const
{
	assert(IsNodeValid(fromNodeId) && "<Graph::GetConnection>: invalid 'from' node index");
	assert(IsNodeValid(toNodeId) && "<Graph::GetConnection>: invalid 'to' node index");

	for (auto& connection : m_uConnections[fromNodeId])
		if (connection && connection->GetToNodeId() == toNodeId)
			return connection.get();

	return nullptr;
}

std::unique_ptr<GraphConnection> Graph::GetConnectionRef(int fromNodeId, int toNodeId)
{
	assert(IsNodeValid(fromNodeId) && "<Graph::GetConnectionRef>: invalid 'from' node index");
	assert(IsNodeValid(toNodeId) && "<Graph::GetConnectionRef>: invalid 'to' node index");

	for (auto& connection : m_uConnections[fromNodeId])
		if (connection && connection->GetToNodeId() == toNodeId)
			return std::move(connection);

	return nullptr;
}

void Graph::RemoveConnection(int fromNodeId, int toNodeId)
{
	assert(IsNodeValid(fromNodeId) && IsNodeValid(toNodeId));

	std::unique_ptr<GraphConnection> conFromTo = GetConnectionRef(fromNodeId, toNodeId);
	std::unique_ptr<GraphConnection> conToFrom = GetConnectionRef(toNodeId, fromNodeId);


	if (!m_bIsDirectional)
	{
		for (auto curEdgeIt = m_uConnections[toNodeId].begin(); curEdgeIt != m_uConnections[toNodeId].end(); ++curEdgeIt)
		{
			if ((*curEdgeIt)->GetToNodeId() == fromNodeId)
			{
				curEdgeIt = m_uConnections[toNodeId].erase(curEdgeIt);
				--m_AmountConnections;
				break;
			}
		}
	}

	for (auto curEdgeIt = m_uConnections[fromNodeId].begin(); curEdgeIt != m_uConnections[fromNodeId].end(); ++curEdgeIt)
	{
		if ((*curEdgeIt)->GetToNodeId() == toNodeId)
		{
			curEdgeIt = m_uConnections[fromNodeId].erase(curEdgeIt);
			--m_AmountConnections;
			break;
		}
	}

	conFromTo.reset();
	conToFrom.reset();

	OnGraphModified(false, true);
}

void Graph::RemoveConnection(std::unique_ptr<GraphConnection> uConnection)
{
	RemoveConnection(uConnection->GetFromNodeId(), uConnection->GetToNodeId());
}

void Graph::RemoveAllConnectionsWithNode(int nodeId)
{
	m_uConnections[nodeId].clear();

	// remove and delete connections from other nodes to this pNode
	auto isConnectionToThisNode = [nodeId](const std::unique_ptr<GraphConnection>& pCon) { return pCon->GetToNodeId() == nodeId; };

	for (std::vector<std::unique_ptr<GraphConnection>>& connection : m_uConnections)
		std::erase_if(connection, isConnectionToThisNode);

	OnGraphModified(false, true);
}

const std::vector<std::unique_ptr<GraphConnection>>& Graph::GetConnectionsFromNode(int nodeId) const
{
	assert(IsNodeValid(nodeId));
	return m_uConnections[nodeId];
}


void Graph::SetConnectionCostsToDistances()
{
	for (const std::vector<std::unique_ptr<GraphConnection>>& connections : m_uConnections)
	{
		for (const std::unique_ptr<GraphConnection>& uConnection : connections)
		{
			glm::vec2 fromPos = GetNode(uConnection->GetFromNodeId())->GetPosition();
			glm::vec2 toPos = GetNode(uConnection->GetToNodeId())->GetPosition();
			uConnection->SetCost(glm::distance(fromPos, toPos));
		}
	}
}


#pragma endregion


#pragma region Private


void Graph::UpdateNextNodeIndex()
{
	int idx = 0;
	while (true)
	{
		if (!IsNodeValid(idx))
		{
			m_NextNodeId = idx;
			return;
		}

		++idx;
	}
}

void Graph::UpdateActiveNodes()
{
	m_pActiveNodes.clear();
	for (auto& uNode : m_uNodes)
	{
		if (uNode != nullptr)
			m_pActiveNodes.push_back(uNode.get());

	}
}


#pragma endregion


#pragma region Protected


void Graph::AddNodeAtIndex(std::unique_ptr<GraphNode> uNode)
{
	uNode->SetId(m_NextNodeId);
	if (m_uNodes.size() < static_cast<size_t>(m_NextNodeId + 1))
	{
		m_uNodes.resize(static_cast<size_t>(m_NextNodeId + 1));
		m_uConnections.resize(static_cast<size_t>(m_NextNodeId + 1));
	}

	m_uNodes[uNode->GetId()] = std::move(uNode);

	++m_AmountNodes;

	UpdateNextNodeIndex();
	UpdateActiveNodes();
}

std::unique_ptr<GraphNode> Graph::CreateNode(const glm::vec2& pos)
{
	if (m_sNodeFactory)
		m_sNodeFactory->CreateNode(pos);

	return std::make_unique<GraphNode>(pos);
}


std::unique_ptr<GraphNode> Graph::CloneNode(const GraphNode& other)
{
	if (m_sNodeFactory)
		m_sNodeFactory->CloneNode(other);

	return std::make_unique<GraphNode>(other);
}


#pragma endregion


