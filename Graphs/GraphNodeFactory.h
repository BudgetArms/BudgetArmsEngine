#pragma once

#include <concepts>

#include "Graphs/GraphNode.h"


namespace bae::Graphs
{
	template<typename T_GraphNode>
	concept GraphNodeType = requires(T_GraphNode node, const glm::vec2 & pos)
	{
		{ T_GraphNode(pos) } -> std::derived_from<GraphNode>;
	};


	class GraphNodeFactory
	{
	public:
		GraphNodeFactory() = default;
		virtual ~GraphNodeFactory() = default;

		virtual GraphNode* const CreateNode(const glm::vec2& pos) const = 0;
		virtual GraphNode* const CloneNode(const GraphNode& other) const = 0;


	};

	template<GraphNodeType T_GraphNode>
	class GraphNodeFactoryTemplate : public GraphNodeFactory
	{
	public:
		GraphNodeFactoryTemplate() = default;
        ~GraphNodeFactoryTemplate() override = default;

		GraphNode* const CreateNode(const glm::vec2& pos) const  override { return new T_GraphNode(pos); }
		GraphNode* const CloneNode(const GraphNode& other) const  override { return  new T_GraphNode(static_cast<const T_GraphNode &>(other)); }


	};
}


