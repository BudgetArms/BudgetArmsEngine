#include <stdexcept>
#include <sstream>
#include <iostream>
#include <chrono> 
#include <thread> 

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#include "BudgetEngine.h"
#include "Managers/InputManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ResourceManager.h"
#include "Core/EventQueue.h"
#include "Core/ServiceLocator.h"

#ifdef STEAMWORKS_ENABLED
#include "Managers/SteamManager.h"
#endif

#include "Singletons/GameTime.h"
#include "Renderer.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "Core/GameObject.h"
#include "Components/Component.h"


void LogSDLVersion(const std::string& message, const SDL_version& v);
void PrintSDLVersion();

SDL_Window* g_window{};

bae::BudgetEngine::BudgetEngine(const std::filesystem::path& dataPath)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());

	g_window = SDL_CreateWindow(
		"BudgetArmsEngine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	if (g_window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);
	ServiceLocator::RegisterSoundSystem(nullptr);
}

bae::BudgetEngine::~BudgetEngine()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void bae::BudgetEngine::Run(const std::function<void()>& load)
{
	load();
	GameTime::GetInstance().Update();

#ifndef __EMSCRIPTEN__
	while (!m_Quit)
		RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

// Game loop
void bae::BudgetEngine::RunOneFrame()
{
	// Get current time & calculate delta time
	GameTime::GetInstance().Update();
	m_AccumlatedTime += GameTime::GetInstance().GetDeltaTime();

	m_Quit = !InputManager::GetInstance().ProcessInput();

#ifdef STEAMWORKS_ENABLED
	SteamManager::GetInstance().Update();
#endif

	// This FixedUpdate system exist for physics and networking
	// eg. if a player runs into a wall, and the game lags for a second,
	// only one updat would be done, resulting into him teleporting through the wall 
	// while here you just do many small updates, that will result in him not teleporting (as much).
	// and for networking, with the syncing of player movement/position & desync, etc. 
	while (m_AccumlatedTime >= GameTime::GetInstance().GetFixedTimeStep())
	{
		SceneManager::GetInstance().FixedUpdate();
		m_AccumlatedTime -= GameTime::GetInstance().GetFixedTimeStep();
	}

	SceneManager::GetInstance().Update();
	SceneManager::GetInstance().LateUpdate();
	Renderer::GetInstance().Render();
	EventQueue::GetInstance().ProcessEvents();

	//std::cout << "FPS: " << static_cast<int>(GameTime::GetInstance().GetFPS()) << '\n';
	std::this_thread::sleep_for(std::chrono::duration<float>(GameTime::GetInstance().GetSleepTime()));

}



void LogSDLVersion(const std::string& message, const SDL_version& v)
{
#if WIN32
	std::stringstream ss;
	ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__

void LoopCallback(void* arg)
{
	static_cast<bae::BudgetEngine*>(arg)->RunOneFrame();
}

#endif


void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	LogSDLVersion("We compiled against SDL version ", version);

	SDL_GetVersion(&version);
	LogSDLVersion("We linked against SDL version ", version);

	SDL_IMAGE_VERSION(&version);
	LogSDLVersion("We compiled against SDL_image version ", version);

	version = *IMG_Linked_Version();
	LogSDLVersion("We linked against SDL_image version ", version);

	SDL_TTF_VERSION(&version)
		LogSDLVersion("We compiled against SDL_ttf version ", version);

	version = *TTF_Linked_Version();
	LogSDLVersion("We linked against SDL_ttf version ", version);
}


