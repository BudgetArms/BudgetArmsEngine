#pragma once

#include <vector>
#include <memory>

#include "Graphs/GraphNode.h"
#include "Graphs/GraphNodeFactory.h"


namespace bae::Graphs
{
#define SAFE_DELETE(p) if (p) { delete (p); (p) = nullptr; }

	class GraphConnection;
	class Graph
	{
	public:
		Graph(bool isDirectional, GraphNodeFactory* const pFactory = nullptr);
		Graph(const Graph& other);
		virtual ~Graph();


		void Clear();
		std::shared_ptr<Graph> Clone() const;

		bool IsDirectional() const { return m_bIsDirectional; }
		int GetNextNodeId() const { return m_NextNodeId; }
		int GetAmountOfConnections() const { return m_AmountConnections; }
		int GetAmountOfNodes() const { return m_AmountNodes; }

		//Nodes
		int AddNode(GraphNode* const pNode);
		void RemoveNode(int nodeId);

		bool IsNodeValid(int nodeId) const;
		GraphNode* const GetNode(int nodeId) const;
		const std::vector<GraphNode*>& GetAllNodes() const { return m_pActiveNodes; }


		//Connections
		void AddConnection(GraphConnection* const pConnection);
		void RemoveConnection(int fromNodeId, int toNodeId);
		void RemoveConnection(GraphConnection* const pConnection);
		void RemoveAllConnectionsWithNode(int nodeId);

		GraphConnection* const GetConnection(int fromNodeId, int toNodeId) const;
		const std::vector<GraphConnection*>& GetConnectionsFromNode(int nodeId) const;
		const std::vector<GraphConnection*>& GetConnectionsFromNode(GraphNode* const pNode) const { return GetConnectionsFromNode(pNode->GetId()); }

		void SetConnectionCostsToDistances();

		//Query nodes and connections
		int GetNodeIdAtPosition(const glm::vec2& position, float errorMargin = 1.f) const;
		GraphNode* const GetNodeAtPosition(const glm::vec2& position, float errorMargin) const;
		bool ConnectionExists(int fromNodeId, int toNodeId) const { return GetConnection(fromNodeId, toNodeId) != nullptr; }

		virtual glm::vec2 GetNodePos(int nodeId) const;
		virtual int GetNodeIdAtPosition(const glm::vec2& position) const { return GetNodeIdAtPosition(position, 1.0f); }
		virtual GraphNode* const GetNodeAtPosition(const glm::vec2& position) const { return GetNodeAtPosition(position, 1.0f); }


	private:
		void UpdateNextNodeIndex();
		void UpdateActiveNodes();


		int m_AmountNodes{ 0 };
		int m_AmountConnections{ 0 };

		const float m_DefaultNodeRadius{ 3.f };


	protected:
		virtual void OnGraphModified(bool nrOfNodesChanged, bool nrOfConnectionsChanged) { nrOfConnectionsChanged; nrOfNodesChanged; };
		void AddNodeAtIndex(GraphNode* const pNode);

		GraphNode* CreateNode(const glm::vec2& pos);
		GraphNode* CloneNode(const GraphNode& other);


		bool m_bIsDirectional;
		int m_NextNodeId{ 0 };


		std::vector<GraphNode*> m_pNodes;
		std::vector<std::vector<GraphConnection*>> m_pConnections;

		std::vector<GraphNode*> m_pActiveNodes;
		std::shared_ptr<GraphNodeFactory> m_pNodeFactory;


	};
}


