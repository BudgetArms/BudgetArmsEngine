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


	/*
	// this should not be done constantly, it's a one time thing
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		std::cout << "AudioChunk: Failed to Initialize Mixer \n";
		throw std::runtime_error(std::string("Failed to Initialize Mixer: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "AudioChunk: Failed to OpenAudio \n";
		throw std::runtime_error(std::string("Failed to OpenAudio: ") + SDL_GetError());
	}

	*/

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

	//Mix_CloseAudio();
	//Mix_Quit();
}


