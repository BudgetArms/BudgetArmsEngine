#include "Keyboard.h"

#include <unordered_map>
#include <ranges>

#include <SDL.h>


using namespace bae;


class Keyboard::Impl
{
public:
	void ProcessInput();
	void ClearCommands();

	void AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState);


	bool IsButtonDown(unsigned int button) const;
	bool IsButtonUp(unsigned int button) const;
	bool IsButtonPressed(unsigned int button) const;


private:
	std::vector<std::tuple<std::unique_ptr<Command>, SDL_Keycode, InputManager::ButtonState>> m_KeyboardCommands{};

	std::unordered_map<SDL_Keycode, bool> m_CurrentKeysDown;
	std::unordered_map<SDL_Keycode, bool> m_PreviousKeysDown;


};



#pragma region Keyboard | NOT PIMPL


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

void Keyboard::ClearCommands()
{
	m_Pimpl->ClearCommands();
}

void Keyboard::AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
	m_Pimpl->AddKeyboardCommands(std::move(command), button, activationState);
}


bool Keyboard::IsButtonUp(unsigned int button) const
{
	return m_Pimpl->IsButtonPressed(button);
}

bool Keyboard::IsButtonDown(unsigned int button) const
{
	return m_Pimpl->IsButtonPressed(button);
}

bool Keyboard::IsButtonPressed(unsigned int button) const
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

	const int maxPeakedEvents{ 128 };
	SDL_Event events[maxPeakedEvents];
	int nrEvents = SDL_PeepEvents(events, maxPeakedEvents, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);

	for (int i = 0; i < nrEvents; i++)
	{
		const SDL_Event& event = events[i];

		if (event.type == SDL_KEYDOWN)
			m_CurrentKeysDown[event.key.keysym.sym] = true;
		else if (event.type == SDL_KEYUP)
			m_CurrentKeysDown[event.key.keysym.sym] = false;

	}


	for (const auto& [command, button, state] : m_KeyboardCommands)
	{
		if (!command)
			throw std::runtime_error("Keyboard::ProcessInput: Command is nullptr");

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

void Keyboard::Impl::ClearCommands()
{
	m_KeyboardCommands.clear();
}

void Keyboard::Impl::AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState)
{
	m_KeyboardCommands.emplace_back(std::move(command), button, activationState);
}

bool Keyboard::Impl::IsButtonDown(unsigned int button) const
{
	bool keyPreviouslyPressed = false;
	bool keyCurrentlyPressed = false;

	if (auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
		keyCurrentlyPressed = buttonIt->second;

	if (auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
		keyPreviouslyPressed = buttonIt->second;

	if (!keyPreviouslyPressed && keyCurrentlyPressed)
		return true;

	return false;
}

bool Keyboard::Impl::IsButtonUp(unsigned int button) const
{
	bool keyPreviouslyPressed = false;
	bool keyCurrentlyPressed = false;

	if (auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
		keyCurrentlyPressed = buttonIt->second;

	if (auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
		keyPreviouslyPressed = buttonIt->second;

	if (keyPreviouslyPressed && !keyCurrentlyPressed)
		return true;

	return false;
}

bool Keyboard::Impl::IsButtonPressed(unsigned int button) const
{
	bool keyPreviouslyPressed = false;
	bool keyCurrentlyPressed = false;

	if (auto buttonIt = m_CurrentKeysDown.find(button); buttonIt != m_CurrentKeysDown.end())
		keyCurrentlyPressed = buttonIt->second;

	if (auto buttonIt = m_PreviousKeysDown.find(button); buttonIt != m_PreviousKeysDown.end())
		keyPreviouslyPressed = buttonIt->second;

	if (keyPreviouslyPressed && keyCurrentlyPressed)
		return true;

	return false;
}


#pragma endregion


