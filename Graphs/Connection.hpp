#pragma once

#include "Graphs/Enums.hpp"


namespace bae::Graphs
{
    class Connection
    {
    public:
        explicit Connection(int fromId, int toId);
        virtual ~Connection() = default;

        [[nodiscard]] int GetFromNodeId() const;
        [[nodiscard]] int GetToNodeId() const;

        [[nodiscard]] Connection GetInverse() const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] bool operator==(const Connection& other) const;


        float m_Weight{ 1.f };

    private:
        const int m_FromNodeId;
        const int m_ToNodeId;
    };
}

