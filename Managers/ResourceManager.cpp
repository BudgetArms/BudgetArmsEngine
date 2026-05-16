#include "ResourceManager.hpp"

#include <stdexcept>

#include <SDL3_ttf/SDL_ttf.h>

#include "Core/HelperFunctions.hpp"
#include "Core/Renderer.hpp"
#include "Wrappers/Font.hpp"
#include "Wrappers/Texture2D.hpp"


namespace fs = std::filesystem;


void bae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
    m_ResourcesPath = dataPath;

    if(!TTF_Init())
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to load support for fonts, Error: ") + SDL_GetError());
    }
}

std::shared_ptr<bae::Texture2D> bae::ResourceManager::LoadTexture(const std::string& file)
{
    const auto fullPath = m_ResourcesPath / file;
    const auto filename = fs::path(fullPath).filename().string();

    if(!m_LoadedTextures.contains(filename))
    {
        m_LoadedTextures.insert(std::pair(filename, std::make_shared<Texture2D>(fullPath.string())));
    }

    return m_LoadedTextures.at(filename);
}

std::shared_ptr<bae::Font> bae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
    const auto fullPath = m_ResourcesPath / file;
    const auto filename = fs::path(fullPath).filename().string();
    const auto key      = std::pair(filename, size);

    if(!m_LoadedFonts.contains(key))
    {
        m_LoadedFonts.insert(std::pair(key, std::make_shared<Font>(fullPath.string(), size)));
    }

    return m_LoadedFonts.at(key);
}

void bae::ResourceManager::UnloadUnusedResources()
{
    for(auto it = m_LoadedTextures.begin(); it != m_LoadedTextures.end();)
    {
        if(it->second.use_count() == 1)
        {
            it = m_LoadedTextures.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for(auto it = m_LoadedFonts.begin(); it != m_LoadedFonts.end();)
    {
        if(it->second.use_count() == 1)
        {
            it = m_LoadedFonts.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


