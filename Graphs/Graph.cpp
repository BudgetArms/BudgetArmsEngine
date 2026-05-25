#include "Graph.hpp"

#include "Connection.hpp"


using namespace bae::Graphs;


Graph::Graph(const bool isDirectional) :
    m_bIsDirectional{ isDirectional }
{
}

Graph::Graph(const Graph& other) :
    m_bIsDirectional{ other.m_bIsDirectional }
{
    m_Nodes.reserve(other.m_Nodes.size());
    for(const std::unique_ptr<Node>& node : other.m_Nodes)
    {
        m_Nodes.push_back(std::make_unique<Node>(*node.get()));
    }

    m_Connections.reserve(other.m_Connections.size());
    for(const std::unique_ptr<Connection>& connection : other.m_Connections)
    {
        m_Connections.push_back(std::make_unique<Connection>(*connection.get()));
    }
}

const std::vector<std::unique_ptr<Node>>& Graph::GetNodes() const
{
    return m_Nodes;
}

std::vector<std::unique_ptr<Node>>& Graph::GetNodes()
{
    return m_Nodes;
}


std::vector<const Node*> Graph::GetActiveNodes() const
{
    std::vector<const Node*> activeNodes{};
    for(const std::unique_ptr<Node>& node : m_Nodes)
    {
        if(node->IsValid())
        {
            activeNodes.push_back(node.get());
        }
    }

    return activeNodes;
}

std::vector<Node*> Graph::GetActiveNodes()
{
    std::vector<Node*> activeNodes{};
    for(const std::unique_ptr<Node>& node : m_Nodes)
    {
        if(node->IsValid())
        {
            activeNodes.push_back(node.get());
        }
    }

    return activeNodes;
}

int Graph::GetNodeCount() const
{
    const auto nodeCount = std::ranges::count_if(m_Nodes,
                                                 [](const auto& node)
                                                 {
                                                     return node->IsValid();
                                                 });
    return static_cast<int>(nodeCount);
}

const std::unique_ptr<Node>& Graph::GetNode(const int nodeId) const
{
    return m_Nodes[nodeId];
}

std::unique_ptr<Node>& Graph::GetNode(const int nodeId)
{
    return m_Nodes[nodeId];
}

int Graph::AddNode(std::unique_ptr<Node> node)
{
    // Check if there's a valid node already in that position
    const auto nodeWithSamePositionIt = std::ranges::find_if(m_Nodes, [&node](const auto& lambdaNode)
    {
        return lambdaNode->IsValid() && node->m_Position == lambdaNode->m_Position;
    });

    if(nodeWithSamePositionIt != m_Nodes.end())
    {
        return InvalidNodeID;
    }

    // replace first invalid node with this node
    const std::optional<int> invalidNodeOpt = GetFirstInvalidNodeId();
    if(invalidNodeOpt.has_value())
    {
        const int invalidNodeId = invalidNodeOpt.value();
        if(invalidNodeId == InvalidNodeID)
        {
            return InvalidNodeID;
        }

        m_Nodes[invalidNodeId].reset();

        node->m_Id             = invalidNodeId;
        m_Nodes[invalidNodeId] = std::move(node);

        return invalidNodeId;
    }

    const int nodeId = static_cast<int>(m_Nodes.size());
    node->m_Id       = nodeId;
    m_Nodes.push_back(std::move(node));

    return nodeId;
}

bool Graph::RemoveNode(const int nodeId)
{
    if(nodeId == InvalidNodeID || nodeId < 0 || nodeId >= m_Nodes.size())
    {
        return false;
    }

    if(m_Nodes[nodeId]->m_Id != nodeId)
    {
        return false;
    }

    // Remove Connections to/from node
    std::erase_if(m_Connections, [&](const auto& connection)
    {
        return connection->GetFromNodeId() == nodeId || connection->GetToNodeId() == nodeId;
    });

    // Invalidate node
    m_Nodes[nodeId]->m_Id = InvalidNodeID;
    return true;
}

const std::vector<std::unique_ptr<Connection>>& Graph::GetConnections() const
{
    return m_Connections;
}

std::vector<std::unique_ptr<Connection>>& Graph::GetConnections()
{
    return m_Connections;
}

Connection* Graph::FindConnection(const int fromNodeId, const int toNodeId)
{
    const auto connectionIt = std::ranges::find_if(m_Connections, [&](const auto& connection)
    {
        return connection->GetFromNodeId() == fromNodeId && connection->GetToNodeId() == toNodeId;
    });

    if(connectionIt == m_Connections.end())
    {
        return nullptr;
    }

    return connectionIt->get();
}

std::vector<Connection*> Graph::FindConnectionsFrom(const int nodeId) const
{
    std::vector<Connection*> connections{};

    for(const auto& connection : m_Connections)
    {
        if(connection->GetFromNodeId() == nodeId)
        {
            connections.push_back(connection.get());
        }
    }

    return connections;
}

std::vector<Connection*> Graph::FindConnectionsTo(const int nodeId) const
{
    std::vector<Connection*> connections{};

    for(const auto& connection : m_Connections)
    {
        if(connection->GetToNodeId() == nodeId)
        {
            connections.push_back(connection.get());
        }
    }

    return connections;
}

std::vector<Connection*> Graph::FindConnectionsWith(const int nodeId) const
{
    std::vector<Connection*> connections{};

    std::vector<Connection*> connectionsFrom = FindConnectionsFrom(nodeId);
    std::vector<Connection*> connectionsTo   = FindConnectionsTo(nodeId);

    std::ranges::move(connectionsFrom, std::back_inserter(connections));
    std::ranges::move(connectionsTo, std::back_inserter(connections));

    return connections;
}

void Graph::AddConnection(std::unique_ptr<Connection> connection)
{
    Connection inverseConnection = connection->GetInverse();

    const auto existingConnectionIt = std::ranges::find_if(m_Connections, [&](const auto& lambdaConnection)
    {
        return connection->GetFromNodeId() == lambdaConnection->GetFromNodeId() &&
                connection->GetToNodeId() == lambdaConnection->GetToNodeId();
    });

    if(existingConnectionIt != m_Connections.end())
    {
        return;
    }

    m_Connections.push_back(std::move(connection));


    if(!m_bIsDirectional)
    {
        const auto existingInverseConnectionIt = std::ranges::find_if(m_Connections, [&](const auto& lambdaConnection)
        {
            return inverseConnection == *lambdaConnection.get();
        });

        if(existingInverseConnectionIt != m_Connections.end())
        {
            return;
        }

        m_Connections.push_back(std::make_unique<Connection>(inverseConnection));
    }
}

void Graph::AddConnection(const int fromNodeId, const int toNodeId)
{
    AddConnection(std::make_unique<Connection>(fromNodeId, toNodeId));
}

bool Graph::RemoveConnection(Connection* const connection)
{
    Connection inverseConnection = connection->GetInverse();

    size_t amountRemoved{};

    amountRemoved += std::erase_if(m_Connections, [&](const auto& lambdaConnection)
    {
        return connection == lambdaConnection.get();
    });

    if(!m_bIsDirectional)
    {
        amountRemoved += std::erase_if(m_Connections, [&](const auto& lambdaConnection)
        {
            return inverseConnection == *lambdaConnection.get();
        });
    }

    return amountRemoved > 0;
}

bool Graph::RemoveConnection(const int fromNodeId, const int toNodeId)
{
    const auto connection = FindConnection(fromNodeId, toNodeId);
    if(!connection)
    {
        return false;
    }

    return RemoveConnection(connection);
}

bool Graph::RemoveConnectionsFrom(int nodeId)
{
    const size_t nrConnectionsRemoved = std::erase_if(m_Connections, [&](const auto& lambdaConnection)
    {
        return lambdaConnection->GetFromNodeId() == nodeId;
    });

    return nrConnectionsRemoved > 0;
}

bool Graph::RemoveConnectionsTo(int nodeId)
{
    const size_t nrConnectionsRemoved = std::erase_if(m_Connections, [&](const auto& lambdaConnection)
    {
        return lambdaConnection->GetToNodeId() == nodeId;
    });

    return nrConnectionsRemoved > 0;
}

bool Graph::GetIsDirectional() const
{
    return m_bIsDirectional;
}

Graph Graph::Clone() const
{
    return Graph{ *this };
}


std::optional<int> Graph::GetFirstInvalidNodeId() const
{
    for(int idx{}; idx < static_cast<int>(m_Nodes.size()); ++idx)
    {
        if(m_Nodes[idx]->m_Id == InvalidNodeID)
        {
            return idx;
        }
    }

    return std::nullopt;
}




