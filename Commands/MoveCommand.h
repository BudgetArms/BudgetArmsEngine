#pragma once
#include "Commands/GameActorCommand.h"
#include "Core/GameObject.h"
#include "Components/TransformComponent.h"
#include "Singletons/GameTime.h"


namespace bae
{
	class MoveCommand final : public GameActorCommand
	{
	public:
		MoveCommand(GameObject& actor, const glm::vec2& direction, float speed = 100.f) :
			GameActorCommand(actor),
			m_Direction{ direction },
			m_Speed{ speed }
		{
			if (glm::length(direction) != 0)
				m_Direction /= glm::length(direction);
		};

		virtual ~MoveCommand() = default;

		MoveCommand(const MoveCommand& other) = delete;
		MoveCommand(MoveCommand&& other) = delete;
		MoveCommand& operator=(const MoveCommand& other) = delete;
		MoveCommand& operator=(MoveCommand&& other) = delete;


		float GetSpeed() const { return m_Speed; }
		void SetSpeed(float speed) { m_Speed = speed; }

		virtual void Execute() override
		{
			auto* transform = GetActor()->GetComponent<TransformComponent>();
			if (!transform)
				return;

			transform->AddLocation({ m_Speed * m_Direction * GameTime::GetInstance().GetDeltaTime(), 0 });
		}


	private:
		glm::vec2 m_Direction;
		float m_Speed;


	};
}


