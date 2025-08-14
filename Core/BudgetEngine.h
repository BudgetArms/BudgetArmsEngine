#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

#include "Core/Utils.h"


namespace bae
{
	class BudgetEngine final
	{
	public:
		explicit BudgetEngine(const bae::Utils::Window& window);
		~BudgetEngine();

		BudgetEngine(const BudgetEngine& other) = delete;
		BudgetEngine(BudgetEngine&& other) = delete;
		BudgetEngine& operator=(const BudgetEngine& other) = delete;
		BudgetEngine& operator=(BudgetEngine&& other) = delete;


		void Run(const std::function<void()>& load);
		void RunOneFrame();


		std::chrono::steady_clock::time_point m_LastTime;
		const float m_FixedTimeStep{ 1.f / 60 };
		const float m_MaxFrames{ 2000 };
		float m_AccumlatedTime{ 0.f }; // m_Lag was too vague/confusing 


	private:
		bool m_Quit{};


	};
}


