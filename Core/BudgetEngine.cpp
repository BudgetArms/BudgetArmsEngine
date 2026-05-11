// VLD include
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

// STD includes
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>


// External libraries includes
#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "HelperFunctions.h"
#include "ServiceLocator.h"

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


// BudgetArmsEngine includes
#include "Core/BudgetEngine.h"
#include "Core/EventQueue.h"
#include "Core/Renderer.h"
#include "Managers/InputManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"
#include "Singletons/GameTime.h"

#ifdef STEAMWORKS_ENABLED
#include "Managers/SteamManager.h"
#endif


#ifdef __EMSCRIPTEN__
void LoopCallback(void* arg)
{
    static_cast<bae::BudgetEngine*>(arg)->RunOneFrame();
}
#endif


void LogSdlVersion(const std::string& message, int major, int minor, int patch);
void PrintSdlVersion();

SDL_Window* g_Window{};


bae::BudgetEngine::BudgetEngine(const Utils::Window& window)
{
    PrintSdlVersion();

    if(!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(
            FUNCTION_NAME + std::string(" Failed to Initialize SDL Video SubSystem, Error: ") + SDL_GetError());
    }


    // Disabled VLD, because MMDevApi.dll leaking (only on my system for some reason)
    #if _DEBUG && __has_include(<vld.h>)
    VLDDisable();
    #endif

    g_Window = SDL_CreateWindow(
        window.Title.c_str(),
        window.Width,
        window.Height,
        SDL_WINDOW_VULKAN
    );

    if(!g_Window)
    {
        g_Window = SDL_CreateWindow(
            window.Title.c_str(),
            window.Width,
            window.Height,
            SDL_WINDOW_OPENGL
        );

        if(!g_Window)
        {
            throw std::runtime_error(
                FUNCTION_NAME + std::string(" Failed to create SDL Window, Error: ") + SDL_GetError());
        }
    }

    #if _DEBUG && __has_include(<vld.h>)
    VLDEnable();
    #endif


    Renderer::GetInstance().Init(g_Window);
    ResourceManager::GetInstance().Init(window.ResourceFolder);
}

bae::BudgetEngine::~BudgetEngine()
{
    std::cout << FUNCTION_NAME << '\n';
    Renderer::GetInstance().Destroy();
    ServiceLocator::RegisterSoundSystem(std::make_unique<NullSoundSystem>());
    SDL_DestroyWindow(g_Window);
    g_Window = nullptr;

    std::cout << FUNCTION_NAME << " Quiting SDL" << '\n';
    SDL_Quit();
}

void bae::BudgetEngine::Run(const std::function<void()>& load)
{
    load();
    GameTime::GetInstance().Update();

    #ifndef __EMSCRIPTEN__
    while(!m_Quit)
    {
        RunOneFrame();
    }
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
    while(m_AccumulatedTime >= GameTime::GetFixedTimeStep())
    {
        SceneManager::GetInstance().FixedUpdate();
        m_AccumulatedTime -= GameTime::GetFixedTimeStep();
    }

    SceneManager::GetInstance().Update();
    SceneManager::GetInstance().LateUpdate();
    Renderer::GetInstance().Render();
    EventQueue::GetInstance().ProcessEvents();

    std::this_thread::sleep_for(std::chrono::duration<float>(GameTime::GetInstance().GetSleepTime()));
}


void LogSdlVersion(const std::string& message, const int major, const int minor, const int patch)
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
void PrintSdlVersion()
{
    LogSdlVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    const int sdlVersion = SDL_GetVersion();
    LogSdlVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(sdlVersion), SDL_VERSIONNUM_MINOR(sdlVersion),
                  SDL_VERSIONNUM_MICRO(sdlVersion));

    LogSdlVersion("Compiled with SDL_ttf ", SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_MICRO_VERSION);
    const int ttfVersion = TTF_Version();
    LogSdlVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(ttfVersion), SDL_VERSIONNUM_MINOR(ttfVersion),
                  SDL_VERSIONNUM_MICRO(ttfVersion));

    LogSdlVersion("Compiled with SDL_mixer ", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION,
                  SDL_MIXER_MICRO_VERSION);
    const int mixerVersion = MIX_Version();
    LogSdlVersion("Linked with SDL_mixer ", SDL_VERSIONNUM_MAJOR(mixerVersion), SDL_VERSIONNUM_MINOR(mixerVersion),
                  SDL_VERSIONNUM_MICRO(mixerVersion));
}

