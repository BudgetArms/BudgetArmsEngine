#pragma once

#include "Singletons/Singleton.hpp"

#include <memory>
#include <vector>


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

        ~InputManager() override; // empty, because forward declaration :D

        InputManager(const InputManager& other)           = delete;
        InputManager(InputManager&& other)                = delete;
        InputManager operator=(InputManager&& other)      = delete;
        InputManager operator=(const InputManager& other) = delete;


        bool ProcessInput() const;
        void ClearCommands() const;

        void AddController(int controllerIndex);
        [[nodiscard]] Controller* GetController(int index) const;
        [[nodiscard]] Keyboard& GetKeyboard() const { return *m_Keyboard; }
        [[nodiscard]] Mouse& GetMouse() const { return *m_Mouse; }

    private:
        friend class Singleton;
        InputManager();

        std::vector<std::unique_ptr<Controller>> m_Controllers;
        std::unique_ptr<Keyboard> m_Keyboard;
        std::unique_ptr<Mouse> m_Mouse;
    };
}


