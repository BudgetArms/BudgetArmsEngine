#include "Keyboard.hpp"

#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include <SDL3/SDL.h>

#include "Core/HelperFunctions.hpp"


using namespace bae;


class Keyboard::Impl
{
public:
    void BeginInput();
    void ProcessEvent(const SDL_Event& event);
    void ExecuteCommands();

    void ClearCommands();

    void AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int key,
                             InputManager::ButtonState activationState);

    [[nodiscard]] bool IsKeyDown(unsigned int key) const;
    [[nodiscard]] bool IsKeyUp(unsigned int key) const;
    [[nodiscard]] bool IsKeyPressed(unsigned int key) const;

private:
    std::vector<std::tuple<std::unique_ptr<Command>, SDL_Keycode, InputManager::ButtonState>> m_KeyboardCommands{};

    std::unordered_map<SDL_Keycode, bool> m_CurrentKeysDown;

    std::unordered_set<SDL_Keycode> m_KeysPressedThisFrame;
    std::unordered_set<SDL_Keycode> m_KeysReleasedThisFrame;
};


#pragma region Keyboard | NOT PIMPL


Keyboard::Keyboard() = default;

Keyboard::~Keyboard() = default;

void Keyboard::BeginInput() const
{
    m_Pimpl->BeginInput();
}

void Keyboard::ProcessEvent(const SDL_Event& event) const
{
    m_Pimpl->ProcessEvent(event);
}

void Keyboard::ExecuteCommands() const
{
    m_Pimpl->ExecuteCommands();
}

void Keyboard::ClearCommands() const
{
    m_Pimpl->ClearCommands();
}

void Keyboard::AddKeyboardCommands(std::unique_ptr<Command> command, const unsigned int key,
                                   const InputManager::ButtonState activationState) const
{
    m_Pimpl->AddKeyboardCommands(std::move(command), key, activationState);
}


bool Keyboard::IsKeyUp(const unsigned int key) const
{
    return m_Pimpl->IsKeyUp(key);
}

bool Keyboard::IsKeyDown(const unsigned int key) const
{
    return m_Pimpl->IsKeyDown(key);
}

bool Keyboard::IsKeyPressed(const unsigned int key) const
{
    return m_Pimpl->IsKeyPressed(key);
}


#pragma endregion


#pragma region Keyboard | PIMPL


void Keyboard::Impl::BeginInput()
{
    m_KeysPressedThisFrame.clear();
    m_KeysReleasedThisFrame.clear();
}

void Keyboard::Impl::ProcessEvent(const SDL_Event& event)
{
    switch(event.type)
    {
        case SDL_EVENT_KEY_DOWN:
        {
            // ignores repeats
            if(event.key.repeat)
            {
                break;
            }

            if(!m_CurrentKeysDown[event.key.key])
            {
                m_CurrentKeysDown[event.key.key] = true;
                m_KeysPressedThisFrame.insert(event.key.key);
            }
        }
        break;
        case SDL_EVENT_KEY_UP:
        {
            if(m_CurrentKeysDown[event.key.key])
            {
                m_CurrentKeysDown[event.key.key] = false;
                m_KeysReleasedThisFrame.insert(event.key.key);
            }
        }
        break;
        default:;
    }
}

void Keyboard::Impl::ExecuteCommands()
{
    // Remove Invalid Commands
    std::erase_if(m_KeyboardCommands, [](auto& keyboardCommand)
    {
        const auto& [command, key, state] = keyboardCommand;
        return !command->IsValid();
    });

    for(const auto& [command, key, state] : m_KeyboardCommands)
    {
        if(!command)
        {
            throw std::runtime_error(FUNCTION_NAME + std::string(" Failed! Command is nullptr"));
        }

        switch(state)
        {
            case InputManager::ButtonState::Down:
            {
                if(IsKeyDown(key))
                {
                    command->Execute();
                }
            }
            break;
            case InputManager::ButtonState::Up:
            {
                if(IsKeyUp(key))
                {
                    command->Execute();
                }
            }
            break;
            case InputManager::ButtonState::Pressed:
            {
                if(IsKeyPressed(key))
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

void Keyboard::Impl::AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int key,
                                         InputManager::ButtonState activationState)
{
    m_KeyboardCommands.emplace_back(std::move(command), key, activationState);
}

bool Keyboard::Impl::IsKeyDown(const unsigned int key) const
{
    return m_KeysPressedThisFrame.contains(key);
}

bool Keyboard::Impl::IsKeyUp(const unsigned int key) const
{
    return m_KeysReleasedThisFrame.contains(key);
}

bool Keyboard::Impl::IsKeyPressed(const unsigned int key) const
{
    if(!m_CurrentKeysDown.contains(key))
    {
        return false;
    }

    const bool isKeyDown = m_CurrentKeysDown.find(key)->second;
    return isKeyDown;
}


#pragma endregion


