#include "Controller.h"


using namespace bae;


class Controller::Impl
{
public:
#ifndef __EMSCRIPTEN__
	void ProcessInput(int controllerIndex);
	void ClearCommands();

	void AddControllerCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState);

	bool IsButtonUp(unsigned int button) const;
	bool IsButtonDown(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;

#else
    void ProcessInput(int) {};
	void ClearCommands() {};

	void AddControllerCommands(std::unique_ptr<Command>, unsigned int, InputManager::ButtonState){};

    bool IsButtonUp(unsigned int) const{ return false; };
	bool IsButtonDown(unsigned int) const{ return false; };
	bool IsButtonPressed(unsigned int) const{  return false; };
#endif


private:
#ifndef __EMSCRIPTEN__
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};
#endif

	std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, InputManager::ButtonState>> m_ControllerCommands{};


};



#pragma region Controller | NOT PIMPL


Controller::Controller(int controllerIndex) :
	m_ControllerIndex{ controllerIndex }
{
}

Controller::~Controller()
{
}

void Controller::ProcessInput()
{
    if (m_Pimpl)
	    m_Pimpl->ProcessInput(m_ControllerIndex);
}

void Controller::ClearCommands()
{
    if (m_Pimpl)
	    m_Pimpl->ClearCommands();
}

void Controller::AddControllerCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
    if (m_Pimpl)
	    m_Pimpl->AddControllerCommands(std::move(command), button, activationState);
}

bool Controller::IsButtonPressed(unsigned int button) const
{
    if (m_Pimpl)
	    return m_Pimpl->IsButtonPressed(button);

    return false;
}


#pragma endregion


#pragma region Controller | PIMPL


#ifndef __EMSCRIPTEN__

void Controller::Impl::ProcessInput(int controllerIndex)
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

void Controller::Impl::ClearCommands()
{
	m_ControllerCommands.clear();
}

void Controller::Impl::AddControllerCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
	m_ControllerCommands.emplace_back(std::make_tuple(std::move(command), button, activationState));
}


bool Controller::Impl::IsButtonUp(unsigned int button) const
{
	const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	const auto buttonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	return (buttonsReleasedThisFrame & button);
};

bool Controller::Impl::IsButtonDown(unsigned int button) const
{
	const auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	const auto buttonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;

	return (buttonsPressedThisFrame & button);
};

bool Controller::Impl::IsButtonPressed(unsigned int button) const
{
	return (m_CurrentState.Gamepad.wButtons & button);
};


#pragma endregion


#endif // ifndef __EMSCRIPTEN__
