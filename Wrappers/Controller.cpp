#include "Controller.hpp"


using namespace bae;


class Controller::Impl
{
public:
    #ifndef __EMSCRIPTEN__
    void ProcessInput(int controllerIndex);
    void ClearCommands();

    void AddControllerCommands(std::unique_ptr<Command> command, unsigned int button,
                               InputManager::ButtonState activationState);

    [[nodiscard]] bool IsButtonUp(unsigned int button) const;
    [[nodiscard]] bool IsButtonDown(unsigned int button) const;
    [[nodiscard]] bool IsButtonPressed(unsigned int button) const;

    #else

    void ProcessInput(int) {};
    void ClearCommands() {};

    void AddControllerCommands(std::unique_ptr<Command>, unsigned int, InputManager::ButtonState) {};

    [[nodiscard]] bool IsButtonUp(unsigned int) const { return false; };
    [[nodiscard]] bool IsButtonDown(unsigned int) const { return false; };
    [[nodiscard]] bool IsButtonPressed(unsigned int) const { return false; };
    #endif

private:
    #ifndef __EMSCRIPTEN__
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};
    #endif

    std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, InputManager::ButtonState>> m_ControllerCommands{};
};


#pragma region Controller | NOT PIMPL


Controller::Controller(const int controllerIndex) :
    m_ControllerIndex{ controllerIndex }
{
}

Controller::~Controller() = default;

void Controller::ProcessInput() const
{
    m_Pimpl->ProcessInput(m_ControllerIndex);
}

void Controller::ClearCommands() const
{
    m_Pimpl->ClearCommands();
}

void Controller::AddControllerCommands(std::unique_ptr<Command> command, const unsigned int button,
                                       const InputManager::ButtonState activationState) const
{
    m_Pimpl->AddControllerCommands(std::move(command), button, activationState);
}

bool Controller::IsButtonUp(unsigned int button) const
{
    return m_Pimpl->IsButtonUp(button);
}

bool Controller::IsButtonDown(unsigned int button) const
{
    return m_Pimpl->IsButtonDown(button);
}

bool Controller::IsButtonPressed(const unsigned int button) const
{
    return m_Pimpl->IsButtonPressed(button);
}


#pragma endregion


#pragma region Controller | PIMPL


#ifndef __EMSCRIPTEN__

void Controller::Impl::ProcessInput(const int controllerIndex)
{
    // m_PreviousState could be a local variable but that would cause a strain
    // on creating a XINPUT_STATE every frame
    CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    XInputGetState(controllerIndex, &m_CurrentState);

    const auto buttonChanges            = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
    const auto buttonsPressedThisFrame  = buttonChanges & m_CurrentState.Gamepad.wButtons;
    const auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

    // Remove Invalid Commands
    std::erase_if(m_ControllerCommands, [](auto& controllerCommand)
    {
        const auto& [command, button, state] = controllerCommand;
        return !command->IsValid();
    });

    for(const auto& [command, button, state] : m_ControllerCommands)
    {
        switch(state)
        {
            case InputManager::ButtonState::Up:
            {
                if(buttonsReleasedThisFrame & button)
                {
                    command->Execute();
                }
            }
            break;
            case InputManager::ButtonState::Down:
            {
                if(buttonsPressedThisFrame & button)
                {
                    command->Execute();
                }
            }
            break;
            case InputManager::ButtonState::Pressed:
            {
                if(m_CurrentState.Gamepad.wButtons & button)
                {
                    command->Execute();
                }
            }
            break;
        }
    }
}

void Controller::Impl::ClearCommands()
{
    m_ControllerCommands.clear();
}

void Controller::Impl::AddControllerCommands(std::unique_ptr<Command> command, unsigned int button,
                                             InputManager::ButtonState activationState)
{
    m_ControllerCommands.emplace_back(std::move(command), button, activationState);
}


bool Controller::Impl::IsButtonUp(const unsigned int button) const
{
    const auto buttonChanges            = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
    const auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

    return buttonsReleasedThisFrame & button;
}

bool Controller::Impl::IsButtonDown(const unsigned int button) const
{
    const auto buttonChanges           = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
    const auto buttonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;

    return buttonsPressedThisFrame & button;
}

bool Controller::Impl::IsButtonPressed(const unsigned int button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}


#pragma endregion


#endif // undef __EMSCRIPTEN__
