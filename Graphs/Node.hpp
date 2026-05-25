#pragma once

#include <glm/glm.hpp>

#include "Core/Utils.hpp"
#include "Graphs/Enums.hpp"


namespace bae::Graphs
{
    class Node
    {
    public:
        explicit Node(const glm::vec2& position);
        virtual ~Node() = default;

        virtual void Render() const;

        [[nodiscard]] virtual bool IsValid() const;

        bool operator==(const Node& node) const;
        bool operator==(const Node* node) const;


        glm::vec2 m_Position;
        int m_Id{ InvalidNodeID };

        const int m_RenderRadius{ 10 };
        Utils::Color m_RenderColor{ Utils::Color::Gray, 0.5f };

    protected:
        friend class Graph;
    };
}


