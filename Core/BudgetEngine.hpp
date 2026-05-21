#pragma once

#include <chrono>
#include <functional>

#include "Core/Utils.hpp"


namespace bae
{
    class BudgetEngine final
    {
    public:
        explicit BudgetEngine(const Utils::Window& window);
        ~BudgetEngine();

        BudgetEngine(const BudgetEngine&)            = delete;
        BudgetEngine(BudgetEngine&&)                 = delete;
        BudgetEngine& operator=(const BudgetEngine&) = delete;
        BudgetEngine& operator=(BudgetEngine&&)      = delete;


        void Run(const std::function<void()>& load);
        void RunOneFrame();


        std::chrono::steady_clock::time_point m_LastTime;
        const float m_FixedTimeStep{ 1.f / 60 };
        const float m_MaxFrames{ 2000.f };
        float m_AccumulatedTime{};

    private:
        bool m_Quit{};
    };
}


