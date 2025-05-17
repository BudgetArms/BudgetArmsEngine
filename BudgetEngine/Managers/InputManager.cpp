#include <SDL.h>
#include "InputManager.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl2.h"

#include "Wrappers/Controller.h"
#include "Wrappers/Keyboard.h"


using namespace bae;


InputManager::InputManager() :
    m_Controllers{ },
    //m_Controllers{ std::make_unique<Controller>(1) },
    m_Keyboard{ std::make_unique<Keyboard>() }
{
    m_Controllers.push_back(std::make_unique<Controller>(0));
}

InputManager::~InputManager()
{
}

bool InputManager::ProcessInput()
{

    // uses SDL_PeekEvents, so not to consume events
    m_Keyboard->ProcessInput();

    // process Keyboard
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return false;

        //process event for IMGUI
        ImGui_ImplSDL2_ProcessEvent(&e);
    }

    for (auto& controller : m_Controllers)
    {
        controller->ProcessInput();
    }



    return true;
}


void InputManager::AddController(int controllerIndex)
{
    m_Controllers.emplace_back(std::make_unique<Controller>(controllerIndex));
}

Controller* InputManager::GetController(int index)
{
    if (m_Controllers.empty())
        return nullptr;

    if (index < 0 || index >= m_Controllers.size())
        return nullptr;

    return m_Controllers[index].get();
}

