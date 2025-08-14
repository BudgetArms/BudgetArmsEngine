#pragma once

#include "Graphs/GraphEnums.h"


namespace bae::Graphs
{
	class GraphConnection
	{
	public:
		GraphConnection(int fromId = InvalidNodeID, int toIndex = InvalidNodeID, float cost = 1.f) :
			m_FromId{ fromId },
			m_ToId{ toIndex },
			m_Cost{ cost }
		{
		}
		virtual ~GraphConnection() = default;


		int GetFromNodeId() const { return m_FromId; }
		void SetFromNodeId(int id) { m_FromId = id; }

		int GetToNodeId() const { return m_ToId; }
		void SetToNodeId(int id) { m_ToId = id; }

		float GetCost() const { return m_Cost; }
		void SetCost(float newCost) { m_Cost = newCost; }

		bool IsValid() const { return (m_FromId != InvalidNodeID && m_ToId != InvalidNodeID); }


		bool operator==(const GraphConnection& rhs) const = default;
		bool operator!=(const GraphConnection& rhs) const = default;


	protected:
		int m_FromId;
		int m_ToId;
		float m_Cost;


	};
}

