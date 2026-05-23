#include "Graph.hpp"

#include <algorithm>
#include <iostream>

#include "Core/HelperFunctions.hpp"
#include "Graphs/GraphConnection.hpp"
#include "Graphs/GraphNode.hpp"


using namespace bae::Graphs;


#pragma region Private


Graph::Graph(const bool isDirectional, GraphNodeFactory* const pNodeFactory) :
    m_bIsDirectional{ isDirectional },
    m_sNodeFactory{ pNodeFactory }
{
}

// Copy constructor
Graph::Graph(const Graph& other) :
    m_AmountNodes{ other.m_AmountNodes },
    m_AmountConnections{ other.m_AmountConnections },
    m_bIsDirectional{ other.m_bIsDirectional },
    m_NextNodeId{ other.m_NextNodeId },
    m_sNodeFactory{ other.m_sNodeFactory }
{
    m_uConnections.clear();

    for(const std::unique_ptr<GraphNode>& uNode : other.m_uNodes)
    {
        if(uNode == nullptr)
        {
            m_uNodes.push_back(nullptr);
        }
        else
        {
            m_uNodes.push_back(CloneNode(*uNode));
        }
    }

    for(const std::vector<std::unique_ptr<GraphConnection>>& connectionList : other.m_uConnections)
    {
        std::vector<std::unique_ptr<GraphConnection>> newList;

        for(const std::unique_ptr<GraphConnection>& uConnection : connectionList)
        {
            newList.push_back(std::make_unique<GraphConnection>(*uConnection));
        }

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
    return std::make_shared<Graph>(*this);
    // return std::shared_ptr<Graph>(new Graph(*this));
}


//== NODES ==
int Graph::AddNode(std::unique_ptr<GraphNode> pNode)
{
    pNode->SetId(m_NextNodeId);
    if(static_cast<int>(m_uNodes.size()) < m_NextNodeId + 1)
    {
        m_uNodes.resize(static_cast<size_t>(m_NextNodeId) + 1);
        m_uConnections.resize(static_cast<size_t>(m_NextNodeId) + 1);
    }

    const int nodeId = pNode->GetId();
    m_uNodes[nodeId] = std::move(pNode);

    ++m_AmountNodes;

    UpdateNextNodeIndex();
    UpdateActiveNodes();

    return nodeId;
}


void Graph::RemoveNode(const int nodeId)
{
    if(!IsNodeValid(nodeId))
    {
        return;
    }

    m_uNodes[nodeId]->SetId(InvalidNodeID);
    m_uNodes[nodeId].reset();
    --m_AmountNodes;


    if(!m_bIsDirectional)
    {
        // visit each neighbor and erase any connections leading to this pNode
        for(auto currentConnectionIt = m_uConnections[nodeId].begin();
            currentConnectionIt != m_uConnections[nodeId].end();
            ++currentConnectionIt)
        {
            for(auto currentEdgeOnToNodeIt = m_uConnections[(*currentConnectionIt)->GetToNodeId()].begin();
                currentEdgeOnToNodeIt != m_uConnections[(*currentConnectionIt)->GetToNodeId()].end();
                ++currentEdgeOnToNodeIt)
            {
                if((*currentEdgeOnToNodeIt)->GetToNodeId() == nodeId)
                {
                    std::unique_ptr<GraphConnection>& conPtr = *currentEdgeOnToNodeIt;
                    currentEdgeOnToNodeIt = m_uConnections[(*currentConnectionIt)->GetToNodeId()].erase(
                        currentEdgeOnToNodeIt);
                    --m_AmountConnections;
                    conPtr.reset();

                    break;
                }
            }
        }
    }

    // finally, clear this pNode's connections
    for(std::unique_ptr<GraphConnection>& pConnection : m_uConnections[nodeId])
    {
        --m_AmountConnections;
        pConnection.reset();
    }

    m_uConnections[nodeId].clear();

    UpdateNextNodeIndex();
    UpdateActiveNodes();
}


bool Graph::IsNodeValid(const int nodeId) const
{
    if(nodeId == InvalidNodeID)
    {
        return false;
    }

    return static_cast<size_t>(nodeId) < m_uNodes.size() && m_uNodes[nodeId] != nullptr;
}

GraphNode* Graph::GetNode(const int nodeId) const
{
    if(!IsNodeValid(nodeId))
    {
        return nullptr;
    }

    return m_uNodes[nodeId].get();
}

std::unique_ptr<GraphNode> Graph::GetNodeRef(const int nodeId)
{
    if(!IsNodeValid(nodeId))
    {
        return nullptr;
    }

    return std::move(m_uNodes[nodeId]);
}

//== CONNECTIONS ==
void Graph::AddConnection(std::unique_ptr<GraphConnection> pConnection)
{
    assert(
        IsNodeValid(pConnection->GetFromNodeId()) && IsNodeValid(pConnection->GetToNodeId()) &&
        "<Graph::AddConnection>: invalid node index");

    const float cost     = pConnection->GetCost();
    const int fromNodeId = pConnection->GetFromNodeId();
    const int toNodeId   = pConnection->GetToNodeId();

    m_uConnections[pConnection->GetFromNodeId()].push_back(std::move(pConnection));
    ++m_AmountConnections;

    if(!m_bIsDirectional)
    {
        auto oppositeConn = std::make_unique<GraphConnection>();
        oppositeConn->SetCost(cost);
        oppositeConn->SetFromNodeId(toNodeId);
        oppositeConn->SetToNodeId(fromNodeId);

        m_uConnections[toNodeId].push_back(std::move(oppositeConn));
        ++m_AmountConnections;
    }
}

void Graph::RemoveConnection(const int fromNodeId, const int toNodeId)
{
    assert(IsNodeValid(fromNodeId) && IsNodeValid(toNodeId));

    std::unique_ptr<GraphConnection> conFromTo = GetConnectionRef(fromNodeId, toNodeId);
    std::unique_ptr<GraphConnection> conToFrom = GetConnectionRef(toNodeId, fromNodeId);


    if(!m_bIsDirectional)
    {
        for(auto curEdgeIt = m_uConnections[toNodeId].begin(); curEdgeIt != m_uConnections[toNodeId].end(); ++curEdgeIt)
        {
            if((*curEdgeIt)->GetToNodeId() == fromNodeId)
            {
                curEdgeIt = m_uConnections[toNodeId].erase(curEdgeIt);
                --m_AmountConnections;
                break;
            }
        }
    }

    for(auto curEdgeIt = m_uConnections[fromNodeId].begin(); curEdgeIt != m_uConnections[fromNodeId].end(); ++curEdgeIt)
    {
        if((*curEdgeIt)->GetToNodeId() == toNodeId)
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

void Graph::RemoveConnection(const std::unique_ptr<GraphConnection>& uConnection)
{
    RemoveConnection(uConnection->GetFromNodeId(), uConnection->GetToNodeId());
}


void Graph::RemoveAllConnectionsWithNode(int nodeId)
{
    m_uConnections[nodeId].clear();

    // remove and delete connections from other nodes to this pNode
    auto isConnectionToThisNode = [nodeId](const std::unique_ptr<GraphConnection>& pCon)
    {
        return pCon->GetToNodeId() == nodeId;
    };

    for(std::vector<std::unique_ptr<GraphConnection>>& connection : m_uConnections)
    {
        std::erase_if(connection, isConnectionToThisNode);
    }

    OnGraphModified(false, true);
}

GraphConnection* Graph::GetConnection(const int fromNodeId, const int toNodeId) const
{
    assert(IsNodeValid(fromNodeId) && "<Graph::GetConnection>: invalid 'from' node index");
    assert(IsNodeValid(toNodeId) && "<Graph::GetConnection>: invalid 'to' node index");

    for(auto& connection : m_uConnections[fromNodeId])
    {
        if(connection && connection->GetToNodeId() == toNodeId)
        {
            return connection.get();
        }
    }

    return nullptr;
}

std::unique_ptr<GraphConnection> Graph::GetConnectionRef(const int fromNodeId, const int toNodeId)
{
    assert(IsNodeValid(fromNodeId) && "<Graph::GetConnectionRef>: invalid 'from' node index");
    assert(IsNodeValid(toNodeId) && "<Graph::GetConnectionRef>: invalid 'to' node index");

    for(auto& connection : m_uConnections[fromNodeId])
    {
        if(connection && connection->GetToNodeId() == toNodeId)
        {
            return std::move(connection);
        }
    }

    return nullptr;
}

const std::vector<std::unique_ptr<GraphConnection>>& Graph::GetConnectionsFromNode(const int nodeId) const
{
    assert(IsNodeValid(nodeId));
    return m_uConnections[nodeId];
}

void Graph::SetConnectionCostsToDistances() const
{
    for(const std::vector<std::unique_ptr<GraphConnection>>& connections : m_uConnections)
    {
        for(const std::unique_ptr<GraphConnection>& uConnection : connections)
        {
            glm::vec2 fromPos = GetNode(uConnection->GetFromNodeId())->GetPosition();
            glm::vec2 toPos   = GetNode(uConnection->GetToNodeId())->GetPosition();
            uConnection->SetCost(glm::distance(fromPos, toPos));
        }
    }
}

int Graph::GetNodeIdAtPosition(const glm::vec2& position, const float errorMargin) const
{
    const GraphNode* const pGraphNode = GetNodeAtPosition(position, errorMargin);
    if(pGraphNode == nullptr)
    {
        return InvalidNodeID;
    }

    return pGraphNode->GetId();
}

GraphNode* Graph::GetNodeAtPosition(const glm::vec2& position, const float errorMargin) const
{
    const float nodeRadius = std::abs(m_DefaultNodeRadius * errorMargin);
    const auto foundIt     = std::ranges::find_if(m_pActiveNodes,
                                              [position, nodeRadius](const GraphNode* pNode)
                                              {
                                                  const float distance = glm::distance(position, pNode->GetPosition());
                                                  return pNode->GetId() != InvalidNodeID && distance < nodeRadius;
                                              });

    if(foundIt != m_pActiveNodes.end())
    {
        return *foundIt;
    }

    return nullptr;
}


glm::vec2 Graph::GetNodePos(const int nodeId) const
{
    if(const auto pNode = GetNode(nodeId))
    {
        return pNode->GetPosition();
    }

    return glm::vec2{ 0, 0 };
}


#pragma endregion


#pragma region Private


void Graph::UpdateNextNodeIndex()
{
    int idx = 0;
    while(true)
    {
        if(!IsNodeValid(idx))
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
    for(auto& uNode : m_uNodes)
    {
        if(uNode != nullptr)
        {
            m_pActiveNodes.push_back(uNode.get());
        }
    }
}


#pragma endregion


#pragma region Protected


// TODO: fix or add logic
// void Graph::OnGraphModified(const bool nrOfNodesChanged, const bool nrOfConnectionsChanged)
void Graph::OnGraphModified(const bool, const bool)
{
}

void Graph::AddNodeAtIndex(std::unique_ptr<GraphNode> uNode)
{
    uNode->SetId(m_NextNodeId);
    if(m_uNodes.size() < static_cast<size_t>(m_NextNodeId) + 1)
    {
        m_uNodes.resize(static_cast<size_t>(m_NextNodeId) + 1);
        m_uConnections.resize(static_cast<size_t>(m_NextNodeId) + 1);
    }

    m_uNodes[uNode->GetId()] = std::move(uNode);

    ++m_AmountNodes;

    UpdateNextNodeIndex();
    UpdateActiveNodes();
}

std::unique_ptr<GraphNode> Graph::CreateNode(const glm::vec2& pos) const
{
    if(m_sNodeFactory)
    {
        if(!m_sNodeFactory->CreateNode(pos))
        {
            std::cout << FUNCTION_NAME << " Node failed to create" << '\n';
        }
    }

    return std::make_unique<GraphNode>(pos);
}


std::unique_ptr<GraphNode> Graph::CloneNode(const GraphNode& other) const
{
    if(m_sNodeFactory)
    {
        if(m_sNodeFactory->CloneNode(other))
        {
            std::cout << FUNCTION_NAME << " Node failed to clone" << '\n';
        }
    }


    return std::make_unique<GraphNode>(other);
}


#pragma endregion


