#pragma once


namespace bae
{
	class State
	{
	public:
		State() = default;
		virtual ~State() = default;


		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

		virtual void Update() = 0;


	};
}


