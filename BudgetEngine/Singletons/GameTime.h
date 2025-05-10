#pragma once
#include <iostream>
#include <chrono>
#include "Singleton.h"


namespace bae
{
    class GameTime final : public Singleton<GameTime>
    {
    public:

        GameTime(const GameTime& other) = delete;
        GameTime(GameTime&& other) = delete;
        GameTime& operator=(const GameTime& other) = delete;
        GameTime& operator=(GameTime&& other) = delete;

        void Update()
        {
            const auto currentTime{ std::chrono::high_resolution_clock::now() };
            m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();

            m_LastTime = currentTime;
        };


        float GetDeltaTime() const { return m_DeltaTime; };
        float GetFixedTimeStep() const { return m_FixedTimeStep; };

        float GetFPS() const { return 1 / m_DeltaTime; };
        float GetSleepTime() const
        {
            constexpr auto delayFrames{ std::chrono::duration<float>(1.f / m_MaxFPS) };
            const std::chrono::duration<float> sleepTime = m_LastTime + delayFrames - std::chrono::high_resolution_clock::now();

            return sleepTime.count();
        };

    private:
        friend class Singleton<GameTime>;
        GameTime() = default;
        ~GameTime() = default;

        static constexpr int m_MaxFPS{ 120 }; //360
        static constexpr float m_FixedTimeStep{ 1 / 60.f };
        float m_DeltaTime{ 1.f };

        std::chrono::high_resolution_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };


    protected:


    };

}


