#pragma once
#include "Singletons/Singleton.h"

#include <vector>
#include <memory>
#include <iostream>


namespace bae
{
	class Controller;
	class Keyboard;
	class Mouse;
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class ButtonState
		{
			Up,
			Down,
			Pressed
		};

		~InputManager(); // empty, because forward declaration :D

		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager operator=(InputManager&& other) = delete;
		InputManager operator=(const InputManager& other) = delete;


		bool ProcessInput();
		void ClearCommands();

		void AddController(int controllerIndex);
		Controller* GetController(int index);
		Keyboard& GetKeyboard() const { return *m_Keyboard; };
		Mouse& GetMouse() const { return *m_Mouse; };



	private:
		friend class Singleton<InputManager>;
		InputManager();

		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::unique_ptr<Keyboard> m_Keyboard;
		std::unique_ptr<Mouse> m_Mouse;


	};
}


