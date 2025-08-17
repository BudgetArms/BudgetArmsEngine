#pragma once

#include <memory>
#include <iostream>

#include "Commands/Command.h"
#include "Managers/InputManager.h"


namespace bae
{
	class Mouse final
	{
	public:
		Mouse();
		~Mouse();

		Mouse(const Mouse& other) = delete;
		Mouse(Mouse&& other) = delete;
		Mouse& operator=(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) = delete;


		void ProcessInput();
		void ClearCommands();

		void AddMouseCommands(std::unique_ptr<Command> command, unsigned int button, InputManager::ButtonState activationState);

		bool IsButtonUp(unsigned int button) const;
		bool IsButtonDown(unsigned int button) const;
		bool IsButtonPressed(unsigned int button) const;


	private:
		// Pimple implementation
		class Impl;
		std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };


	};
}


