#pragma once

#include <concepts>

#include "Graphs/Node.hpp"


namespace bae::Graphs
{
    class NodeFactory
    {
    public:
        NodeFactory()          = default;
        virtual ~NodeFactory() = default;

        virtual std::unique_ptr<Node> CreateNode(const glm::vec2& Position) const = 0;
        virtual std::unique_ptr<Node> CloneNode(const Node& Other) const = 0;
    };


    template<typename NodeType>
        requires std::derived_from<NodeType, Node>
    class NodeFactoryTemplate final : public NodeFactory
    {
    public:
        NodeFactoryTemplate()           = default;
        ~NodeFactoryTemplate() override = default;


        [[nodiscard]] std::unique_ptr<Node> CreateNode(const glm::vec2& pos) const override
        {
            return std::make_unique<NodeType>(pos);
        }

        [[nodiscard]] std::unique_ptr<Node> CloneNode(const Node& other) const override
        {
            return std::make_unique<NodeType>(static_cast<const NodeType&>(other));
        }
    };
}


