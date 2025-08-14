#include "ServiceLocator.h"

#include "Sounds/NullAudioClip.h"
#include "Sounds/SdlSoundSystem.h"

using namespace bae;

std::unique_ptr<IAudioQueue> ServiceLocator::m_sAudioQueueInstance = std::make_unique<AudioQueue<NullAudioClip>>();

std::unique_ptr<SoundSystem> ServiceLocator::m_sSoundSystemInstance = std::make_unique<NullSoundSystem>();


