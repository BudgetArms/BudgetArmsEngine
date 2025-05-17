#include "Keyboard.h"

#include <iostream>
#include <array>
#include <unordered_map>
#include <ranges>

//#include "Managers/InputManager.h"

using namespace bae;


class Keyboard::Impl
{
public:
    void TESTING()
    {
        std::cout << "nice\n";
    }

    void ProcessInput()
    {
        //std::ranges::copy(m_CurrentKeysDown, m_PreviousKeysDown.begin());
        m_PreviousKeysDown = m_CurrentKeysDown;

        // Gets newest event information
        SDL_PumpEvents();

        const int maxPeakedEvents{ 16 };
        SDL_Event events[maxPeakedEvents];
        int nrEvents = SDL_PeepEvents(events, maxPeakedEvents, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

        for (int i = 0; i < nrEvents; i++)
        {
            const SDL_Event& event = events[i];
            if (event.type == SDL_KEYDOWN && !event.key.repeat)
                m_CurrentKeysDown[event.key.keysym.sym] = true;
            else if (event.type == SDL_KEYUP)
                m_CurrentKeysDown[event.key.keysym.sym] = false;

        }


        //for (const auto& [command, button, state] : m_KeyboardCommands)
        for (auto& [command, button, state] : m_KeyboardCommands)
        {
            bool keyPreviouslyPressed = false;
            bool keyCurrentlyPressed = false;

            if (auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
                keyCurrentlyPressed = buttonIt->second;

            if (auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
                keyPreviouslyPressed = buttonIt->second;


            switch (state)
            {
            case InputManager::ButtonState::Down:
            {
                if (!keyPreviouslyPressed && keyCurrentlyPressed)
                    //std::cout << "Down\n\n\n\n\n\n\n";
                    command->Execute();

            } break;
            case InputManager::ButtonState::Up:
            {
                if (keyPreviouslyPressed && !keyCurrentlyPressed)
                    //std::cout << "Up\n";
                    command->Execute();
            } break;
            case InputManager::ButtonState::Pressed:
            {
                if (keyPreviouslyPressed && keyCurrentlyPressed)
                    //std::cout << "Pressed\n";
                    command->Execute();
            } break;
            }

        }


    }


    void AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
    {
        m_KeyboardCommands.emplace_back(std::move(command), button, activationState);
    }


private:
    std::vector<std::tuple<std::unique_ptr<Command>, SDL_Keycode, InputManager::ButtonState>> m_KeyboardCommands{};

    std::unordered_map<SDL_Keycode, bool> m_CurrentKeysDown;
    std::unordered_map<SDL_Keycode, bool> m_PreviousKeysDown;


protected:


};


Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::ProcessInput()
{
    m_Pimpl->ProcessInput();
}

void Keyboard::AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
    m_Pimpl->AddKeyboardCommands(std::move(command), button, activationState);
}


