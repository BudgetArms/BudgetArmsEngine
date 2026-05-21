#pragma once

#include <memory>

#include "Commands/Command.hpp"
#include "Managers/InputManager.hpp"


namespace bae
{
    class Keyboard final
    {
    public:
        Keyboard();
        ~Keyboard();

        Keyboard(const Keyboard&)            = delete;
        Keyboard(Keyboard&&)                 = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(Keyboard&&)      = delete;


        void ProcessInput() const;
        void ClearCommands() const;

        void AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int button,
                                 InputManager::ButtonState activationState) const;

        [[nodiscard]] bool IsButtonUp(unsigned int button) const;
        [[nodiscard]] bool IsButtonDown(unsigned int button) const;
        [[nodiscard]] bool IsButtonPressed(unsigned int button) const;

    private:
        // Pimple implementation
        class Impl;
        std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };
    };
}


