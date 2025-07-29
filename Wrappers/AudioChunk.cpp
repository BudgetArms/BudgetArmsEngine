#include "AudioChunk.h"

#include <stdexcept>
#include <iostream>
#include <filesystem>

#include <SDL_mixer.h>


Mix_Chunk* bae::AudioChunk::GetChunk() const
{
	return m_Chunk;
}

bae::AudioChunk::AudioChunk(const std::string& fullPath) :
	m_Chunk{ nullptr }
{

	if (!std::filesystem::exists(fullPath))
	{
		std::cout << "AudioChunk: File path not found\n";
		throw std::runtime_error(std::string("File path not found: "));
	}


	m_Chunk = Mix_LoadWAV(fullPath.c_str());
	if (!m_Chunk)
	{
		std::cout << "AudioChunk: Failed to load WAV file\n";
		throw std::runtime_error(std::string("Failed to load Chunk: ") + SDL_GetError());
	}

}

bae::AudioChunk::~AudioChunk()
{
	Mix_FreeChunk(m_Chunk);
	m_Chunk = nullptr;
}


