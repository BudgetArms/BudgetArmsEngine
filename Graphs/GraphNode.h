#pragma once

#include <glm.hpp>

#include "Core/Utils.h"
#include "Graphs/GraphEnums.h"


namespace bae::Graphs
{
	class GraphNode
	{
	public:
		GraphNode(const glm::vec2& pos = {}) :
			m_Position{ pos },
			m_Id{ InvalidNodeID }
		{
		}
		virtual ~GraphNode() = default;

		virtual void Render() const
		{
			bae::Utils::FillCircle(m_Position, m_RenderRadius, m_RenderColor);
		}


		int GetId() const { return m_Id; };

		glm::vec2 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec2& pos) { m_Position = pos; }

		int m_RenderRadius{ 10 };
		bae::Utils::Color m_RenderColor{ bae::Utils::Color::Gray, 0.5f };


	protected:
		friend class Graph;

		void SetId(int id) { m_Id = id; }


		glm::vec2 m_Position;
		int m_Id;


	};
}


