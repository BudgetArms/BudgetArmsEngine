#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>

#include "Singletons/Singleton.h"


namespace bae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::filesystem::path& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, uint8_t size);

		std::filesystem::path GetResourcesPath() const { return m_ResourcesPath; }


	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		void UnloadUnusedResources();


		std::filesystem::path m_ResourcesPath;

		std::map<std::string, std::shared_ptr<Texture2D>> m_loadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::shared_ptr<Font>> m_loadedFonts;


	};
}


