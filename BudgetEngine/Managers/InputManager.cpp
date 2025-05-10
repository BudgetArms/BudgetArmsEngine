#include <SDL.h>
#include "InputManager.h"

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl2.h"



bool bae::InputManager::ProcessInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return false;

        if (e.type == SDL_KEYDOWN)
        {
            std::cout << "Keydown" << '\n';


        }
        else if (e.type == SDL_KEYUP)
        {
            if (e.key.keysym.scancode == SDL_GetScancodeFromKey(SDLK_ESCAPE))
                return false;



        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {


        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {


        }

        //process event for IMGUI
        ImGui_ImplSDL2_ProcessEvent(&e);

        // etc...

    }

    return true;
}

