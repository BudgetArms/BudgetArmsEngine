#include "Graph.h"

#include "Graphs/GraphNode.h"
#include "Graphs/GraphConnection.h"


using namespace bae::Graphs;


#pragma region Private


Graph::Graph(bool isDirectional, GraphNodeFactory* const pNodeFactory)
	:m_bIsDirectional{ isDirectional }
	, m_NextNodeId{ 0 }
	, m_pNodes{}
	, m_pConnections{}
	, m_pNodeFactory{ pNodeFactory }
{
}

// Copy constructor
Graph::Graph(const Graph& other)
	:m_pNodes{  }
	, m_pConnections{ }
	, m_AmountConnections{ other.m_AmountConnections }
	, m_AmountNodes{ other.m_AmountNodes }
	, m_NextNodeId{ other.m_NextNodeId }
	, m_bIsDirectional{ other.m_bIsDirectional }
	, m_pNodeFactory{ other.m_pNodeFactory }
{
	for (std::vector<GraphConnection*>& connectionList : m_pConnections)
	{
		for (GraphConnection* pConnection : connectionList)
			SAFE_DELETE(pConnection);
	}

	for (GraphNode* pNode : other.m_pNodes)
	{
		if (pNode == nullptr)
			m_pNodes.push_back(nullptr);
		else
			m_pNodes.push_back(CloneNode(*pNode));

	}

	for (const std::vector<GraphConnection*>& connectionList : other.m_pConnections)
	{
		std::vector<GraphConnection*> newList;
		for (const GraphConnection* pConnection : connectionList)
			newList.push_back(new GraphConnection(*pConnection));
		m_pConnections.push_back(newList);
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
	for (GraphNode* pNode : m_pNodes)
	{
		SAFE_DELETE(pNode);
	}

	for (std::vector<GraphConnection*>& connections : m_pConnections)
	{
		for (GraphConnection* pConnection : connections)
		{
			SAFE_DELETE(pConnection);
		}
		connections.clear();
	}
	m_pConnections.clear();
	m_pNodes.clear();
	m_NextNodeId = 0;
}


std::shared_ptr<Graph> Graph::Clone() const
{
	return std::shared_ptr<Graph>(new Graph(*this));
}




//== NODES == 
int Graph::AddNode(GraphNode* const pNode)
{
	pNode->SetId(m_NextNodeId);
	const int size = static_cast<int>(m_pNodes.size());
	if (size < m_NextNodeId + 1)
	{
		m_pNodes.resize(m_NextNodeId + 1);
		m_pConnections.resize(m_NextNodeId + 1);
	}
	m_pNodes[pNode->GetId()] = pNode;

	++m_AmountNodes;

	UpdateNextNodeIndex();
	UpdateActiveNodes();

	return pNode->GetId();
}


void Graph::RemoveNode(int index)
{
	if (!IsNodeValid(index))
		return;

	GraphNode* node = m_pNodes[index];
	node->SetId(InvalidNodeID);
	SAFE_DELETE(node);
	m_pNodes[index] = nullptr;

	--m_AmountNodes;

	bool hadConnections{ false };

	//if the graph is not directed remove all connections leading to this pNode and then
	//clear the connections leading from the pNode
	if (!m_bIsDirectional)
	{
		//visit each neighbour and erase any connections leading to this pNode
		for (auto currentConnection = m_pConnections[index].begin();
			currentConnection != m_pConnections[index].end();
			++currentConnection)
		{
			for (auto currentEdgeOnToNode = m_pConnections[(*currentConnection)->GetToNodeId()].begin();
				currentEdgeOnToNode != m_pConnections[(*currentConnection)->GetToNodeId()].end();
				++currentEdgeOnToNode)
			{
				if ((*currentEdgeOnToNode)->GetToNodeId() == index)
				{
					hadConnections = true;

					GraphConnection* conPtr = *currentEdgeOnToNode;
					currentEdgeOnToNode = m_pConnections[(*currentConnection)->GetToNodeId()].erase(currentEdgeOnToNode);
					--m_AmountConnections;
					SAFE_DELETE(conPtr);

					break;
				}
			}
		}
	}

	//finally, clear this pNode's connections
	for (GraphConnection* pConnection : m_pConnections[index])
	{
		--m_AmountConnections;
		hadConnections = true;
		SAFE_DELETE(pConnection);
	}
	m_pConnections[index].clear();

	UpdateNextNodeIndex();
	UpdateActiveNodes();
}


bool Graph::IsNodeValid(int index) const
{
	return (size_t)index < m_pNodes.size() && m_pNodes[index] != nullptr;
}

GraphNode* const Graph::GetNode(int index) const
{
	if (!IsNodeValid(index))
		return nullptr;

	return m_pNodes[index];
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
	auto foundIt = find_if(m_pActiveNodes.begin(), m_pActiveNodes.end(),
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

	return { 0, 0 };
}


//== CONNECTIONS ==
void Graph::AddConnection(GraphConnection* const pConnection)
{
	assert(IsNodeValid(pConnection->GetFromNodeId()) && IsNodeValid(pConnection->GetToNodeId()) && "<Graph::AddConnection>: invalid node index");

	m_pConnections[pConnection->GetFromNodeId()].push_back(pConnection);
	++m_AmountConnections;

	if (!m_bIsDirectional)
	{
		GraphConnection* oppositeConn = new GraphConnection();
		oppositeConn->SetCost(pConnection->GetCost());
		oppositeConn->SetFromNodeId(pConnection->GetToNodeId());
		oppositeConn->SetToNodeId(pConnection->GetFromNodeId());

		m_pConnections[pConnection->GetToNodeId()].push_back(oppositeConn);
		++m_AmountConnections;
	}
}

GraphConnection* const Graph::GetConnection(int from, int to) const
{
	assert(IsNodeValid(from) && "<Graph::GetConnection>: invalid 'from' node index");
	assert(IsNodeValid(to) && "<Graph::GetConnection>: invalid 'to' node index");
	for (auto c : m_pConnections[from])
	{
		if (c && c->GetToNodeId() == to)
			return c;
	}

	return nullptr;
}

void Graph::RemoveConnection(int from, int to)
{
	assert(IsNodeValid(from) && IsNodeValid(to));

	GraphConnection* conFromTo = GetConnection(from, to);
	GraphConnection* conToFrom = GetConnection(to, from);

	if (!m_bIsDirectional)
	{
		for (auto curEdge = m_pConnections[to].begin();
			curEdge != m_pConnections[to].end();
			++curEdge)
		{
			if ((*curEdge)->GetToNodeId() == from)
			{
				curEdge = m_pConnections[to].erase(curEdge);
				--m_AmountConnections;
				break;
			}
		}
	}

	for (auto curEdge = m_pConnections[from].begin();
		curEdge != m_pConnections[from].end();
		++curEdge)
	{
		if ((*curEdge)->GetToNodeId() == to)
		{
			curEdge = m_pConnections[from].erase(curEdge);
			--m_AmountConnections;
			break;
		}
	}

	SAFE_DELETE(conFromTo);
	SAFE_DELETE(conToFrom);

	OnGraphModified(false, true);
}

void Graph::RemoveConnection(GraphConnection* const pConnection)
{
	RemoveConnection(pConnection->GetFromNodeId(), pConnection->GetToNodeId());
}

void Graph::RemoveAllConnectionsWithNode(int nodeId)
{
	for (auto c : m_pConnections[nodeId])
		delete c;
	m_pConnections[nodeId].clear();

	// remove and delete connections from other nodes to this pNode
	auto isConnectionToThisNode = [nodeId](GraphConnection* const pCon) { return pCon->GetToNodeId() == nodeId; };
	for (std::vector<GraphConnection*>& c : m_pConnections)
	{
		std::vector<GraphConnection*>::iterator foundIt{};
		while ((foundIt = std::find_if(c.begin(), c.end(), isConnectionToThisNode)) != c.end())
		{
			delete* foundIt;
			c.erase(foundIt);
		}
	}

	OnGraphModified(false, true);
}

const std::vector<GraphConnection*>& Graph::GetConnectionsFromNode(int nodeId) const
{
	assert(IsNodeValid(nodeId));
	return m_pConnections[nodeId];
}


void Graph::SetConnectionCostsToDistances()
{
	for (const std::vector<GraphConnection*>& connections : m_pConnections)
	{
		for (GraphConnection* const pConnection : connections)
		{
			glm::vec2 fromPos = GetNode(pConnection->GetFromNodeId())->GetPosition();
			glm::vec2 toPos = GetNode(pConnection->GetToNodeId())->GetPosition();
			pConnection->SetCost(glm::distance(fromPos, toPos));
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
	for (auto& n : m_pNodes)
	{
		if (n != nullptr)
		{
			m_pActiveNodes.push_back(n);
		}
	}
}


#pragma endregion


#pragma region Protected


void Graph::AddNodeAtIndex(GraphNode* const pNode)
{
	pNode->SetId(m_NextNodeId);
	if (m_pNodes.size() < (size_t)m_NextNodeId + 1)
	{
		m_pNodes.resize(m_NextNodeId + 1);
		m_pConnections.resize(m_NextNodeId + 1);
	}
	m_pNodes[pNode->GetId()] = pNode;

	++m_AmountNodes;

	UpdateNextNodeIndex();
	UpdateActiveNodes();
}

GraphNode* Graph::CreateNode(const glm::vec2& pos)
{
	if (m_pNodeFactory)
		m_pNodeFactory->CreateNode(pos);

	return new GraphNode(pos);
}


GraphNode* Graph::CloneNode(const GraphNode& other)
{
	if (m_pNodeFactory)
		m_pNodeFactory->CloneNode(other);

	return new GraphNode(other);
}


#pragma endregion


