#include "Connection.hpp"


using namespace bae::Graphs;


Connection::Connection(const int fromId, const int toId) :
    m_FromNodeId{ fromId },
    m_ToNodeId{ toId }
{
}

int Connection::GetFromNodeId() const
{
    return m_FromNodeId;
}

int Connection::GetToNodeId() const
{
    return m_ToNodeId;
}

Connection Connection::GetInverse() const
{
    Connection connection{ m_ToNodeId, m_FromNodeId };
    connection.m_Weight = m_Weight;

    return connection;
}

bool Connection::IsValid() const
{
    return m_FromNodeId != InvalidNodeID && m_ToNodeId != InvalidNodeID;
}

bool Connection::operator==(const Connection& other) const
{
    return m_FromNodeId == other.m_FromNodeId && m_ToNodeId == other.m_ToNodeId;
}
