#pragma once

#include "Components/ImguiComponent.h"


namespace bae
{
    class GameObject;
    class TrashTheCacheComponent final : public ImguiComponent
    {
    public:
        explicit TrashTheCacheComponent(GameObject& owner, bool isExercise2);
        ~TrashTheCacheComponent() = default;

        TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
        TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
        TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
        TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;


        virtual void RenderGUI() override;


    private:
        void TimingExercise1();
        void TimingExercise2();
        void TimingExercise2Alt();


        bool m_bIsExercise2{ false };
        int m_NrSamples;

        const int m_BufferSize{ static_cast<int>(std::pow(2, 26)) };

        std::vector<float> m_TimingsExercise1;
        std::vector<float> m_TimingsExercise2;
        std::vector<float> m_TimingsExercise2Alt;


    protected:


    };
}


