#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <stdexcept>
#include <sstream>
#include <iostream>
#include <chrono> 
#include <thread> 

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

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

void LoopCallback(void* arg)
{
	static_cast<bae::BudgetEngine*>(arg)->RunOneFrame();
}

#endif


void LogSDLVersion(const std::string& message, int major, int minor, int patch);
void PrintSDLVersion();

SDL_Window* g_window{};


bae::BudgetEngine::BudgetEngine(const bae::Utils::Window& window)
{
	PrintSDLVersion();

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());


	// Disabled VLD, because MMDevApi.dll leaking (only on my system for some reason)
#if defined(_DEBUG) && __has_include(<vld.h>)
	VLDDisable();
#endif

	g_window = SDL_CreateWindow(
		window.title.c_str(),
		window.width,
		window.height,
		SDL_WINDOW_VULKAN
	);

#if defined(_DEBUG) && __has_include(<vld.h>)
	VLDEnable();
#endif

	if (g_window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());


	if (window.bIsVSyncOn)
	{
		if (SDL_GL_SetSwapInterval(1) == false)
		{
			std::cerr << "BudgetEngine::BudgetEngine, error when calling SDL_GL_SetSwapInterval: " << SDL_GetError() << std::endl;
			return;
		}
	}
	else
		SDL_GL_SetSwapInterval(0);


	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(window.resourceFolder);
}

bae::BudgetEngine::~BudgetEngine()
{
	Renderer::GetInstance().Destroy();
    MIX_Quit();
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
	m_AccumulatedTime += GameTime::GetInstance().GetDeltaTime();

	m_Quit = !InputManager::GetInstance().ProcessInput();

#ifdef STEAMWORKS_ENABLED
	SteamManager::GetInstance().Update();
#endif

	// This FixedUpdate system exist for physics and networking
	// e.g. if a player runs into a wall, and the game lags for a second,
	// only one update would be done, resulting into him teleporting through the wall
	// while here you just do many small updates, that will result in him not teleporting (as much).
	// and for networking, with the syncing of player movement/position & desync, etc. 
	while (m_AccumulatedTime >= GameTime::GetInstance().GetFixedTimeStep())
	{
		SceneManager::GetInstance().FixedUpdate();
		m_AccumulatedTime -= GameTime::GetInstance().GetFixedTimeStep();
	}

	SceneManager::GetInstance().Update();
	SceneManager::GetInstance().LateUpdate();
	Renderer::GetInstance().Render();
	EventQueue::GetInstance().ProcessEvents();

	std::this_thread::sleep_for(std::chrono::duration<float>(GameTime::GetInstance().GetSleepTime()));
}




void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}


// Why bother with this, because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	const int sdlVersion = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(sdlVersion), SDL_VERSIONNUM_MINOR(sdlVersion), SDL_VERSIONNUM_MICRO(sdlVersion));

	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
	const int ttfVersion = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(ttfVersion), SDL_VERSIONNUM_MINOR(ttfVersion),	SDL_VERSIONNUM_MICRO(ttfVersion));

	LogSDLVersion("Compiled with SDL_mixer ",	SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_MICRO_VERSION);
	const int mixerVersion = MIX_Version();
	LogSDLVersion("Linked with SDL_mixer ", SDL_VERSIONNUM_MAJOR(mixerVersion), SDL_VERSIONNUM_MINOR(mixerVersion),	SDL_VERSIONNUM_MICRO(mixerVersion));


}

