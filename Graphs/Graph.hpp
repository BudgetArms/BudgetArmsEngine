#pragma once

#include <memory>
#include <vector>

#include "Graphs/Connection.hpp"
#include "Graphs/Node.hpp"


namespace bae::Graphs
{
    class Graph
    {
    public:
        explicit Graph(bool isDirectional);
        explicit Graph(const Graph& other);
        virtual ~Graph() = default;


        // Nodes
        [[nodiscard]] const std::vector<std::unique_ptr<Node>>& GetNodes() const;
        [[nodiscard]] std::vector<std::unique_ptr<Node>>& GetNodes();
        [[nodiscard]] std::vector<const Node*> GetActiveNodes() const;
        [[nodiscard]] std::vector<Node*> GetActiveNodes();
        [[nodiscard]] int GetNodeCount() const;

        const std::unique_ptr<Node>& GetNode(int nodeId) const;
        std::unique_ptr<Node>& GetNode(int nodeId);


        template<typename T>
        T const* GetNodeAs(const int nodeId) const
        {
            return static_cast<T const*>(m_Nodes[nodeId].get());
        }

        template<typename T>
        T* GetNodeAs(const int nodeId)
        {
            return static_cast<T*>(m_Nodes[nodeId].get());
        }

        // Return Type: (new) NodeId of the Node
        // NodeID of node will be changed
        int AddNode(std::unique_ptr<Node> node);
        bool RemoveNode(int nodeId);


        // Connections
        const std::vector<std::unique_ptr<Connection>>& GetConnections() const;
        std::vector<std::unique_ptr<Connection>>& GetConnections();

        [[nodiscard]] Connection* FindConnection(int fromNodeId, int toNodeId);
        [[nodiscard]] std::vector<Connection*> FindConnectionsFrom(int nodeId) const;
        [[nodiscard]] std::vector<Connection*> FindConnectionsTo(int nodeId) const;
        [[nodiscard]] std::vector<Connection*> FindConnectionsWith(int nodeId) const;

        void AddConnection(std::unique_ptr<Connection> connection);
        void AddConnection(int fromNodeId, int toNodeId);

        bool RemoveConnection(Connection* connection);
        bool RemoveConnection(int fromNodeId, int toNodeId);

        bool RemoveConnectionsFrom(int nodeId);
        bool RemoveConnectionsTo(int nodeId);


        // Graph
        [[nodiscard]] bool GetIsDirectional() const;
        [[nodiscard]] Graph Clone() const;

    protected:
        std::optional<int> GetFirstInvalidNodeId() const;


        bool m_bIsDirectional;

        std::vector<std::unique_ptr<Node>> m_Nodes{};
        std::vector<std::unique_ptr<Connection>> m_Connections{};
    };
}


