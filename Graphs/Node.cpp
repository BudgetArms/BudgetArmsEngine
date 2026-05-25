#include "Node.hpp"


using namespace bae::Graphs;


Node::Node(const glm::vec2& position) :
    m_Position{ position }
{
}

void Node::Render() const
{
    bae::Utils::FillCircle(m_Position, m_RenderRadius, m_RenderColor);
}

bool Node::IsValid() const
{
    return m_Id != InvalidNodeID;
}

bool Node::operator==(const Node& node) const
{
    return m_Id == node.m_Id;
}

bool Node::operator==(const Node* node) const
{
    return m_Id == node->m_Id;
}
