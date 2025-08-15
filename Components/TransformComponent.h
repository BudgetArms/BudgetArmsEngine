#pragma once
#include <glm.hpp>
#include <iostream>

#include "Components/Component.h"


namespace bae
{
	class GameObject;
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject& owner);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;


		const glm::vec2& GetWorldLocation();
		float GetWorldRotation();
		const glm::vec2& GetWorldScale();

		const glm::vec2& GetLocalLocation() const;
		float GetLocalRotation() const;
		const glm::vec2& GetLocalScale() const;

		void SetWorldLocation(const glm::vec2& location);
		void SetWorldRotation(float rotation);
		void SetWorldScale(const glm::vec2& scale);

		void SetLocalLocation(const glm::vec2& location);
		void SetLocalRotation(float rotation);
		void SetLocalScale(const glm::vec2& scale);

		void AddLocation(const glm::vec2& addLocation);
		void AddRotation(float addRotation);
		void AddScale(const glm::vec2& addScale);


		// trivial setter, so should actually be removed
		constexpr void SetLocationDirty() { m_bLocationDirty = true; };
		constexpr void SetRotationDirty() { m_bRotationDirty = true; };
		constexpr void SetScaleDirty() { m_bScaleDirty = true; };


	private:
		void UpdateWorldLocation();
		void UpdateWorldRotation();
		void UpdateWorldScale();


		glm::vec2 m_WorldLocation{};
		glm::vec2 m_WorldScale{ 1, 1 };
		float m_WorldRotation{};

		glm::vec2 m_LocalLocation{};
		glm::vec2 m_LocalScale{ 1, 1 };
		float m_LocalRotation{};

		bool m_bLocationDirty{ false };
		bool m_bRotationDirty{ false };
		bool m_bScaleDirty{ false };


	};
}


