#include "InputManager.hpp"
#include <SDL3/SDL.h>

#include <imgui_impl_sdl3.h>

#include "Wrappers/Controller.hpp"
#include "Wrappers/Keyboard.hpp"
#include "Wrappers/Mouse.hpp"

#ifdef STEAMWORKS_ENABLED
#include "Managers/SteamManager.hpp"
#endif


using namespace bae;


InputManager::~InputManager() = default;

bool InputManager::ProcessInput()
{
    // uses SDL_PeekEvents, so not to consume events
    m_Keyboard->ProcessInput();
    m_Mouse->ProcessInput();

    // process Keyboard
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_EVENT_QUIT)
        {
            return false;
        }

        #ifdef STEAMWORKS_ENABLED
        if(e.key.key == SDLK_PERIOD)
        {
            SteamManager::GetInstance().ResetAllAchievements();
        }
        #endif


        // Todo: remove ImGui
        //process event for IMGUI
        ImGui_ImplSDL3_ProcessEvent(&e);
    }

    for(const std::unique_ptr<Controller>& controller : m_Controllers)
    {
        controller->ProcessInput();
    }


    return true;
}

void InputManager::ClearCommands() const
{
    for(const std::unique_ptr<Controller>& controller : m_Controllers)
    {
        controller->ClearCommands();
    }

    if(m_Keyboard)
    {
        m_Keyboard->ClearCommands();
    }

    if(m_Mouse)
    {
        m_Mouse->ClearCommands();
    }
}

void InputManager::AddController(int controllerIndex)
{
    m_Controllers.emplace_back(std::make_unique<Controller>(controllerIndex));
}


Controller* InputManager::GetController(const int index) const
{
    if(m_Controllers.empty())
    {
        return nullptr;
    }

    if(index < 0 || index >= static_cast<int>(m_Controllers.size()))
    {
        return nullptr;
    }

    return m_Controllers[index].get();
}

InputManager::InputManager() :
    m_Keyboard{ std::make_unique<Keyboard>() },
    m_Mouse{ std::make_unique<Mouse>() }
{
    m_Controllers.push_back(std::make_unique<Controller>(0));
}


