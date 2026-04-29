#pragma once

#include "Graphs/GraphEnums.h"


namespace bae::Graphs
{
    class GraphConnection final
    {
    public:
        explicit GraphConnection(const int fromId = InvalidNodeID, const int toIndex = InvalidNodeID,
                                 const float cost = 1.f) :
            m_FromId{ fromId },
            m_ToId{ toIndex },
            m_Cost{ cost }
        {
        }

        ~GraphConnection() = default;


        [[nodiscard]] int GetFromNodeId() const { return m_FromId; }
        void SetFromNodeId(const int id) { m_FromId = id; }

        [[nodiscard]] int GetToNodeId() const { return m_ToId; }
        void SetToNodeId(const int id) { m_ToId = id; }

        [[nodiscard]] float GetCost() const { return m_Cost; }
        void SetCost(const float newCost) { m_Cost = newCost; }

        [[nodiscard]] bool IsValid() const
        {
            return m_FromId != InvalidNodeID && m_ToId != InvalidNodeID;
        }


        bool operator==(const GraphConnection& rhs) const = default;
        bool operator!=(const GraphConnection& rhs) const = default;

    protected:
        int m_FromId{};
        int m_ToId{};
        float m_Cost{};
    };
}

