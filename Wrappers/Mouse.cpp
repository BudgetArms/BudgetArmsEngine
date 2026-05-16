#include "Mouse.hpp"

#include <ranges>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

#include "Core/HelperFunctions.hpp"


using namespace bae;


class Mouse::Impl
{
public:
    void ProcessInput();
    void ClearCommands();

    void AddMouseCommands(std::unique_ptr<Command> command, unsigned int button,
                          InputManager::ButtonState activationState);

    [[nodiscard]] bool IsButtonDown(unsigned int button) const;
    [[nodiscard]] bool IsButtonUp(unsigned int button) const;
    [[nodiscard]] bool IsButtonPressed(unsigned int button) const;

private:
    std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, InputManager::ButtonState>> m_MouseCommands{};

    std::unordered_map<unsigned int, bool> m_CurrentButtonsDown;
    std::unordered_map<unsigned int, bool> m_PreviousButtonsDown;
};


#pragma region Mouse | NOT PIMPL


Mouse::Mouse() = default;

Mouse::~Mouse() = default;

void Mouse::ProcessInput() const
{
    m_Pimpl->ProcessInput();
}

void Mouse::ClearCommands() const
{
    m_Pimpl->ClearCommands();
}

void Mouse::AddMouseCommands(std::unique_ptr<Command> command, const unsigned int button,
                             const InputManager::ButtonState activationState) const
{
    m_Pimpl->AddMouseCommands(std::move(command), button, activationState);
}


bool Mouse::IsButtonUp(const unsigned int button) const
{
    return m_Pimpl->IsButtonUp(button);
}

bool Mouse::IsButtonDown(const unsigned int button) const
{
    return m_Pimpl->IsButtonDown(button);
}

bool Mouse::IsButtonPressed(const unsigned int button) const
{
    return m_Pimpl->IsButtonPressed(button);
}


#pragma endregion


#pragma region Mouse | PIMPL


void Mouse::Impl::ProcessInput()
{
    m_PreviousButtonsDown = m_CurrentButtonsDown;


    const Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

    m_CurrentButtonsDown[SDL_BUTTON_LEFT]   = (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
    m_CurrentButtonsDown[SDL_BUTTON_MIDDLE] = (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) != 0;
    m_CurrentButtonsDown[SDL_BUTTON_RIGHT]  = (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0;
    m_CurrentButtonsDown[SDL_BUTTON_X1]     = (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_X1)) != 0;
    m_CurrentButtonsDown[SDL_BUTTON_X2]     = (mouseState & SDL_BUTTON_MASK(SDL_BUTTON_X2)) != 0;

    // Remove Invalid Commands
    std::erase_if(m_MouseCommands, [](auto& mouseCommand)
    {
        const auto& [command, button, state] = mouseCommand;
        return !command->IsValid();
    });

    for(const auto& [command, button, state] : m_MouseCommands)
    {
        if(!command)
        {
            throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Command is nullptr"));
        }

        bool keyPreviouslyPressed = false;
        bool keyCurrentlyPressed  = false;

        if(auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
        {
            keyCurrentlyPressed = buttonIt->second;
        }

        if(auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
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

void Mouse::Impl::ClearCommands()
{
    m_MouseCommands.clear();
}

void Mouse::Impl::AddMouseCommands(std::unique_ptr<Command> command, unsigned int button,
                                   InputManager::ButtonState activationState)
{
    m_MouseCommands.emplace_back(std::move(command), button, activationState);
}

bool Mouse::Impl::IsButtonDown(const unsigned int button) const
{
    bool keyPreviouslyPressed = false;
    bool keyCurrentlyPressed  = false;

    if(const auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
    {
        keyCurrentlyPressed = buttonIt->second;
    }

    if(const auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
    {
        keyPreviouslyPressed = buttonIt->second;
    }

    if(!keyPreviouslyPressed && keyCurrentlyPressed)
    {
        return true;
    }

    return false;
}

bool Mouse::Impl::IsButtonUp(const unsigned int button) const
{
    bool keyPreviouslyPressed = false;
    bool keyCurrentlyPressed  = false;

    if(const auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
    {
        keyCurrentlyPressed = buttonIt->second;
    }

    if(const auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
    {
        keyPreviouslyPressed = buttonIt->second;
    }

    if(keyPreviouslyPressed && !keyCurrentlyPressed)
    {
        return true;
    }

    return false;
}

bool Mouse::Impl::IsButtonPressed(const unsigned int button) const
{
    bool keyPreviouslyPressed = false;
    bool keyCurrentlyPressed  = false;

    if(const auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
    {
        keyCurrentlyPressed = buttonIt->second;
    }

    if(const auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
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



