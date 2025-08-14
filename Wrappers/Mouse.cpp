#include "Mouse.h"

#include <unordered_map>
#include <vector>
#include <ranges>

#include <SDL.h>


using namespace bae;


class Mouse::Impl
{
public:
	void ProcessInput();

	void AddMouseCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState);

	bool IsButtonDown(unsigned int button) const;
	bool IsButtonUp(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;


private:
	std::vector<std::tuple<std::unique_ptr<Command>, unsigned int, InputManager::ButtonState>> m_MouseCommands{};

	std::unordered_map<unsigned int, bool> m_CurrentButtonsDown;
	std::unordered_map<unsigned int, bool> m_PreviousButtonsDown;


};



#pragma region Mouse | NOT PIMPL


Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

void Mouse::ProcessInput()
{
	m_Pimpl->ProcessInput();
}

void Mouse::AddMouseCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
	m_Pimpl->AddMouseCommands(std::move(command), button, activationState);
}


bool Mouse::IsButtonUp(unsigned int button) const
{
	return m_Pimpl->IsButtonPressed(button);
}

bool Mouse::IsButtonDown(unsigned int button) const
{
	return m_Pimpl->IsButtonPressed(button);
}

bool Mouse::IsButtonPressed(unsigned int button) const
{
	return m_Pimpl->IsButtonPressed(button);
}


#pragma endregion


#pragma region Mouse | PIMPL


void Mouse::Impl::ProcessInput()
{
	m_PreviousButtonsDown = m_CurrentButtonsDown;


	Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

	m_CurrentButtonsDown[SDL_BUTTON_LEFT] = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
	m_CurrentButtonsDown[SDL_BUTTON_MIDDLE] = (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
	m_CurrentButtonsDown[SDL_BUTTON_RIGHT] = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
	m_CurrentButtonsDown[SDL_BUTTON_X1] = (mouseState & SDL_BUTTON(SDL_BUTTON_X1)) != 0;
	m_CurrentButtonsDown[SDL_BUTTON_X2] = (mouseState & SDL_BUTTON(SDL_BUTTON_X2)) != 0;


	for (const auto& [command, button, state] : m_MouseCommands)
	{
		if (!command)
			throw std::runtime_error("Mouse::ProcessInput: Command is nullptr");

		bool keyPreviouslyPressed = false;
		bool keyCurrentlyPressed = false;

		if (auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
			keyCurrentlyPressed = buttonIt->second;

		if (auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
			keyPreviouslyPressed = buttonIt->second;


		switch (state)
		{
			case InputManager::ButtonState::Down:
			{
				if (!keyPreviouslyPressed && keyCurrentlyPressed)
					command->Execute();
			} break;
			case InputManager::ButtonState::Up:
			{
				if (keyPreviouslyPressed && !keyCurrentlyPressed)
					command->Execute();
			} break;
			case InputManager::ButtonState::Pressed:
			{
				if (keyPreviouslyPressed && keyCurrentlyPressed)
					command->Execute();
			} break;
		}
	}

}


void Mouse::Impl::AddMouseCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
	m_MouseCommands.emplace_back(std::move(command), button, activationState);
}

bool Mouse::Impl::IsButtonDown(unsigned int button) const
{
	bool keyPreviouslyPressed = false;
	bool keyCurrentlyPressed = false;

	if (auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
		keyCurrentlyPressed = buttonIt->second;

	if (auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
		keyPreviouslyPressed = buttonIt->second;

	if (!keyPreviouslyPressed && keyCurrentlyPressed)
		return true;

	return false;
}

bool Mouse::Impl::IsButtonUp(unsigned int button) const
{
	bool keyPreviouslyPressed = false;
	bool keyCurrentlyPressed = false;

	if (auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
		keyCurrentlyPressed = buttonIt->second;

	if (auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
		keyPreviouslyPressed = buttonIt->second;

	if (keyPreviouslyPressed && !keyCurrentlyPressed)
		return true;

	return false;
}

bool Mouse::Impl::IsButtonPressed(unsigned int button) const
{
	bool keyPreviouslyPressed = false;
	bool keyCurrentlyPressed = false;

	if (auto buttonIt = m_CurrentButtonsDown.find(button); buttonIt != m_CurrentButtonsDown.end())
		keyCurrentlyPressed = buttonIt->second;

	if (auto buttonIt = m_PreviousButtonsDown.find(button); buttonIt != m_PreviousButtonsDown.end())
		keyPreviouslyPressed = buttonIt->second;

	if (keyPreviouslyPressed && keyCurrentlyPressed)
		return true;

	return false;
}


#pragma endregion



