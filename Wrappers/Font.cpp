#include <stdexcept>

#include <SDL_ttf.h>

#include "Font.h"


bae::Font::Font(const std::string& fullPath, unsigned int size) : m_font(nullptr)
{
	m_font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_font == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

bae::Font::~Font()
{
	TTF_CloseFont(m_font);
}


TTF_Font* bae::Font::GetFont() const
{
	return m_font;
}


