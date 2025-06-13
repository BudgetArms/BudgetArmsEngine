#pragma once
#include <memory>
#include "Core/SoundSystem.h"
#include "Core/NullSoundSystem.h"


namespace bae
{
    class ServiceLocator final
    {
    public:
        static SoundSystem& GetSoundSystem() { return *m_sSoundSystemInstance; }
        static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& soundSystem)
        {
            if (!soundSystem)
                m_sSoundSystemInstance = std::make_unique<NullSoundSystem>();
            else
                m_sSoundSystemInstance = std::move(soundSystem);
        }


    private:
        static std::unique_ptr<SoundSystem> m_sSoundSystemInstance;


    };
}


