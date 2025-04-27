#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include <chrono>

namespace bae
{
    class BudgetEngine final
    {
        bool m_Quit{};
    public:
        explicit BudgetEngine(const std::filesystem::path& dataPath);
        ~BudgetEngine();
        void Run(const std::function<void()>& load);
        void RunOneFrame();

        BudgetEngine(const BudgetEngine& other) = delete;
        BudgetEngine(BudgetEngine&& other) = delete;
        BudgetEngine& operator=(const BudgetEngine& other) = delete;
        BudgetEngine& operator=(BudgetEngine&& other) = delete;


        // VARIALBES:
        std::chrono::steady_clock::time_point m_LastTime;
        const float m_FixedTimeStep{ 1.f / 60 };
        const float m_MaxFrames{ 2000 };
        float m_AccumlatedTime{ 0.f }; // m_Lag was too vague/confusing 

    };
}