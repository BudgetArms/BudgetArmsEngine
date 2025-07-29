#pragma once
#include <string>

struct Mix_Chunk;
namespace bae
{
    /**
     * Simple RAII wrapper for a Mix_Chunk
     */
     // TODO: Note that this could be a abstract class with templates, so that
     // you could have SdlAudioChunk, and you would hide that implementation with Pimpl
     // And then you could have XAudioChunk, for another audio system
     // And then SoundSystem or more specifically SdlSoundSystem::Impl uses AudioChunk
     // it could be in SoundSystem, if AudioChunk was not tied to Sdl_Mixer

    // Time constrains don't let me change this without maybe breaking and then fixing other part
    // No Time To Die (eg. fix bugs)

    class AudioChunk final
    {
    public:
        Mix_Chunk* GetChunk() const;
        explicit AudioChunk(const std::string& fullPath);
        ~AudioChunk();

        AudioChunk(const AudioChunk&) = delete;
        AudioChunk(AudioChunk&&) = delete;
        AudioChunk& operator= (const AudioChunk&) = delete;
        AudioChunk& operator= (const AudioChunk&&) = delete;
    private:
        Mix_Chunk* m_Chunk;
    };
}
