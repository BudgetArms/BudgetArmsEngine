#include "ServiceLocator.h"


using namespace bae;


std::unique_ptr<SoundSystem> ServiceLocator::m_sSoundSystemInstance = std::make_unique<NullSoundSystem>();
std::unique_ptr<AudioQueue> ServiceLocator::m_sAudioQueueInstance = std::make_unique<AudioQueue>();


