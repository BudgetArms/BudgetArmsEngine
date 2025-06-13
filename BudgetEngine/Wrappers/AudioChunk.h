#pragma once
#include <string>

struct Mix_Chunk;
namespace bae
{
    /**
     * Simple RAII wrapper for a _Mix_Chunk
     */
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
