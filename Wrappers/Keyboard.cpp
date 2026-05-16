#include "Keyboard.hpp"

#include <ranges>
#include <stdexcept>
#include <unordered_map>

#include <SDL3/SDL.h>

#include "Core/HelperFunctions.hpp"


using namespace bae;


class Keyboard::Impl
{
public:
    void ProcessInput();
    void ClearCommands();

    void AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button,
                             InputManager::ButtonState activationState);

    [[nodiscard]] bool IsButtonDown(unsigned int button) const;
    [[nodiscard]] bool IsButtonUp(unsigned int button) const;
    [[nodiscard]] bool IsButtonPressed(unsigned int button) const;

private:
    std::vector<std::tuple<std::unique_ptr<Command>, SDL_Keycode, InputManager::ButtonState>> m_KeyboardCommands{};

    std::unordered_map<SDL_Keycode, bool> m_CurrentKeysDown;
    std::unordered_map<SDL_Keycode, bool> m_PreviousKeysDown;
};


#pragma region Keyboard | NOT PIMPL


Keyboard::Keyboard() = default;

Keyboard::~Keyboard() = default;

void Keyboard::ProcessInput() const
{
    m_Pimpl->ProcessInput();
}

void Keyboard::ClearCommands() const
{
    m_Pimpl->ClearCommands();
}

void Keyboard::AddKeyboardCommands(std::unique_ptr<Command> command, const unsigned int button,
                                   const InputManager::ButtonState activationState) const
{
    m_Pimpl->AddKeyboardCommands(std::move(command), button, activationState);
}


bool Keyboard::IsButtonUp(const unsigned int button) const
{
    return m_Pimpl->IsButtonUp(button);
}

bool Keyboard::IsButtonDown(const unsigned int button) const
{
    return m_Pimpl->IsButtonDown(button);
}

bool Keyboard::IsButtonPressed(const unsigned int button) const
{
    return m_Pimpl->IsButtonPressed(button);
}


#pragma endregion


#pragma region Keyboard | PIMPL


void Keyboard::Impl::ProcessInput()
{
    m_PreviousKeysDown = m_CurrentKeysDown;

    // Gets newest event information
    SDL_PumpEvents();

    constexpr int maxPeakedEvents{ 128 };
    SDL_Event events[maxPeakedEvents];
    const int nrEvents = SDL_PeepEvents(events, maxPeakedEvents, SDL_PEEKEVENT, SDL_EVENT_FIRST, SDL_EVENT_LAST);

    for(int i = 0; i < nrEvents; i++)
    {
        const SDL_Event& event = events[i];

        if(event.type == SDL_EVENT_KEY_DOWN)
        {
            m_CurrentKeysDown[event.key.key] = true;
        }
        else if(event.type == SDL_EVENT_KEY_UP)
        {
            m_CurrentKeysDown[event.key.key] = false;
        }
    }

    // Remove Invalid Commands
    std::erase_if(m_KeyboardCommands, [](auto& keyboardCommand)
    {
        const auto& [command, button, state] = keyboardCommand;
        return !command->IsValid();
    });

    for(const auto& [command, button, state] : m_KeyboardCommands)
    {
        if(!command)
        {
            throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Command is nullptr"));
        }

        bool keyPreviouslyPressed = false;
        bool keyCurrentlyPressed  = false;

        if(auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
        {
            keyCurrentlyPressed = buttonIt->second;
        }

        if(auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
        {
            keyPreviouslyPressed = buttonIt->second;
        }


        switch(state)
        {
            case InputManager::ButtonState::Down:
            {
                if(!keyPreviouslyPressed && keyCurrentlyPressed)
                {
                    command->Execute();
                }
            }
            break;
            case InputManager::ButtonState::Up:
            {
                if(keyPreviouslyPressed && !keyCurrentlyPressed)
                {
                    command->Execute();
                }
            }
            break;
            case InputManager::ButtonState::Pressed:
            {
                if(keyPreviouslyPressed && keyCurrentlyPressed)
                {
                    command->Execute();
                }
            }
            break;
        }
    }
}

void Keyboard::Impl::ClearCommands()
{
    m_KeyboardCommands.clear();
}

void Keyboard::Impl::AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button,
                                         InputManager::ButtonState activationState)
{
    m_KeyboardCommands.emplace_back(std::move(command), button, activationState);
}

bool Keyboard::Impl::IsButtonDown(const unsigned int button) const
{
    bool keyPreviouslyPressed = false;
    bool keyCurrentlyPressed  = false;

    if(const auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
    {
        keyCurrentlyPressed = buttonIt->second;
    }

    if(const auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
    {
        keyPreviouslyPressed = buttonIt->second;
    }

    if(!keyPreviouslyPressed && keyCurrentlyPressed)
    {
        return true;
    }

    return false;
}

bool Keyboard::Impl::IsButtonUp(const unsigned int button) const
{
    bool keyPreviouslyPressed = false;
    bool keyCurrentlyPressed  = false;

    if(const auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
    {
        keyCurrentlyPressed = buttonIt->second;
    }

    if(const auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
    {
        keyPreviouslyPressed = buttonIt->second;
    }

    if(keyPreviouslyPressed && !keyCurrentlyPressed)
    {
        return true;
    }

    return false;
}

bool Keyboard::Impl::IsButtonPressed(const unsigned int button) const
{
    bool keyPreviouslyPressed = false;
    bool keyCurrentlyPressed  = false;

    if(const auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
    {
        keyCurrentlyPressed = buttonIt->second;
    }

    if(const auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
    {
        keyPreviouslyPressed = buttonIt->second;
    }

    if(keyPreviouslyPressed && keyCurrentlyPressed)
    {
        return true;
    }

    return false;
}


#pragma endregion


