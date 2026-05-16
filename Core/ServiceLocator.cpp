#include "ServiceLocator.hpp"

#include "Sounds/MixerSoundSystem.hpp"


using namespace bae;

std::unique_ptr<SoundSystem> ServiceLocator::m_sSoundSystemInstance = std::make_unique<NullSoundSystem>();


