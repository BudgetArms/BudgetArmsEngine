#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>

#include "Singletons/Singleton.hpp"


namespace bae
{
    class Texture2D;
    class Font;

    class ResourceManager final : public Singleton<ResourceManager>
    {
    public:
        void Init(const std::filesystem::path& dataPath);
        std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
        std::shared_ptr<Font> LoadFont(const std::string& file, uint8_t size);

        [[nodiscard]] std::filesystem::path GetResourcesPath() const { return m_ResourcesPath; }

    private:
        friend class Singleton;
        ResourceManager() = default;

        void UnloadUnusedResources();


        std::filesystem::path m_ResourcesPath;

        std::map<std::string, std::shared_ptr<Texture2D>> m_LoadedTextures;
        std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_LoadedFonts;
    };
}


