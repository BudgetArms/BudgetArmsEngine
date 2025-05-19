#include "Controller.h"

using namespace bae;



class Controller::Impl
{
public:
    void ProcessInput(int controllerIndex)
    {
        // m_PreviousState could be a local variable but that would cause a strain 
        // on creating a XINPUT_STATE every frame
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(controllerIndex, &m_CurrentState);

        const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        const auto buttonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
        const auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);


        for (const auto& [command, button, state] : m_ControllerCommands)
        {
            switch (state)
            {
            case InputManager::ButtonState::Up:
            {
                if (buttonsReleasedThisFrame & button)
                    command->Execute();
            } break;
            case InputManager::ButtonState::Down:
            {
                if (buttonsPressedThisFrame & button)
                    command->Execute();

            } break;
            case InputManager::ButtonState::Pressed:
            {
                if (m_CurrentState.Gamepad.wButtons & button)
                    command->Execute();
            } break;
            }
        }


    }

    void AddControllerCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
    {
        m_ControllerCommands.emplace_back(std::make_tuple(std::move(command), button, activationState));
    }


    bool IsButtonUp(unsigned int button) const
    {
        const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        const auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

        return (buttonsReleasedThisFrame & button);
    };

    bool IsButtonDown(unsigned int button) const
    {
        const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
        const auto buttonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;

        return (buttonsPressedThisFrame & button);
    };

    bool IsButtonPressed(unsigned int button) const
    {
        return (m_CurrentState.Gamepad.wButtons & button);
    };

private:
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};

    std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, InputManager::ButtonState>> m_ControllerCommands;


protected:


};


Controller::Controller(int controllerIndex) :
    m_ControllerIndex{ controllerIndex }
{
}

Controller::~Controller()
{
}

void Controller::ProcessInput()
{
    m_Pimpl->ProcessInput(m_ControllerIndex);
}

void Controller::AddControllerCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
    m_Pimpl->AddControllerCommands(std::move(command), button, activationState);
}

bool Controller::IsButtonPressed(unsigned int button) const
{
    return m_Pimpl->IsButtonPressed(button);
}


