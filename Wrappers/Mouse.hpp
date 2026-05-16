#pragma once

#include <memory>

#include "Commands/Command.hpp"
#include "Managers/InputManager.hpp"


namespace bae
{
    class Mouse final
    {
    public:
        Mouse();
        ~Mouse();

        Mouse(const Mouse& other)            = delete;
        Mouse(Mouse&& other)                 = delete;
        Mouse& operator=(const Mouse& other) = delete;
        Mouse& operator=(Mouse&& other)      = delete;


        void ProcessInput() const;
        void ClearCommands() const;

        void AddMouseCommands(std::unique_ptr<Command> command, unsigned int button,
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


