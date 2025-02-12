#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	class GameEngine final
	{
		bool m_quit{};
	public:
		explicit GameEngine(const std::filesystem::path& dataPath);
		~GameEngine();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		GameEngine(const GameEngine& other) = delete;
		GameEngine(GameEngine&& other) = delete;
		GameEngine& operator=(const GameEngine& other) = delete;
		GameEngine& operator=(GameEngine&& other) = delete;
	};
}