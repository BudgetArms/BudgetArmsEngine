#pragma once

#include <memory>
#include <vector>

#include "Graphs/GraphNode.h"
#include "Graphs/GraphNodeFactory.h"


namespace bae::Graphs
{
    class GraphConnection;

    class Graph
    {
    public:
        explicit Graph(bool isDirectional, GraphNodeFactory* pNodeFactory = nullptr);
        Graph(const Graph& other);
        virtual ~Graph();


        void Clear();
        [[nodiscard]] std::shared_ptr<Graph> Clone() const;

        [[nodiscard]] bool IsDirectional() const { return m_bIsDirectional; }
        [[nodiscard]] int GetNextNodeId() const { return m_NextNodeId; }
        [[nodiscard]] int GetAmountOfConnections() const { return m_AmountConnections; }
        [[nodiscard]] int GetAmountOfNodes() const { return m_AmountNodes; }

        //Nodes
        int AddNode(std::unique_ptr<GraphNode> pNode);
        void RemoveNode(int nodeId);

        [[nodiscard]] bool IsNodeValid(int nodeId) const;
        [[nodiscard]] GraphNode* GetNode(int nodeId) const;
        std::unique_ptr<GraphNode> GetNodeRef(int nodeId);
        [[nodiscard]] const std::vector<GraphNode*>& GetAllNodes() const { return m_pActiveNodes; }


        //Connections
        void AddConnection(std::unique_ptr<GraphConnection> pConnection);
        void RemoveConnection(int fromNodeId, int toNodeId);
        void RemoveConnection(const std::unique_ptr<GraphConnection>& uConnection);
        void RemoveAllConnectionsWithNode(int nodeId);

        [[nodiscard]] GraphConnection* GetConnection(int fromNodeId, int toNodeId) const;
        std::unique_ptr<GraphConnection> GetConnectionRef(int fromNodeId, int toNodeId);
        [[nodiscard]] const std::vector<std::unique_ptr<GraphConnection>>& GetConnectionsFromNode(int nodeId) const;

        const std::vector<std::unique_ptr<GraphConnection>>& GetConnectionsFromNode(const GraphNode* const pNode) const
        {
            return GetConnectionsFromNode(pNode->GetId());
        }

        void SetConnectionCostsToDistances() const;

        //Query nodes and connections
        [[nodiscard]] int GetNodeIdAtPosition(const glm::vec2& position, float errorMargin = 1.f) const;
        [[nodiscard]] GraphNode* GetNodeAtPosition(const glm::vec2& position, float errorMargin) const;

        [[nodiscard]] bool ConnectionExists(const int fromNodeId, const int toNodeId) const
        {
            return GetConnection(fromNodeId, toNodeId) != nullptr;
        }

        [[nodiscard]] virtual glm::vec2 GetNodePos(int nodeId) const;

        [[nodiscard]] virtual int GetNodeIdAtPosition(const glm::vec2& position) const
        {
            return GetNodeIdAtPosition(position, 1.0f);
        }

        [[nodiscard]] virtual GraphNode* GetNodeAtPosition(const glm::vec2& position) const
        {
            return GetNodeAtPosition(position, 1.0f);
        }

    private:
        void UpdateNextNodeIndex();
        void UpdateActiveNodes();


        int m_AmountNodes{ 0 };
        int m_AmountConnections{ 0 };

        const float m_DefaultNodeRadius{ 3.f };

    protected:
        virtual void OnGraphModified(bool nrOfNodesChanged, bool nrOfConnectionsChanged);
        void AddNodeAtIndex(std::unique_ptr<GraphNode> uNode);

        [[nodiscard]] std::unique_ptr<GraphNode> CreateNode(const glm::vec2& pos) const;
        [[nodiscard]] std::unique_ptr<GraphNode> CloneNode(const GraphNode& other) const;


        bool m_bIsDirectional;
        int m_NextNodeId{ 0 };

        std::vector<std::unique_ptr<GraphNode>> m_uNodes{};
        std::vector<std::vector<std::unique_ptr<GraphConnection>>> m_uConnections{};

        std::vector<GraphNode*> m_pActiveNodes;
        std::shared_ptr<GraphNodeFactory> m_sNodeFactory;
    };
}


