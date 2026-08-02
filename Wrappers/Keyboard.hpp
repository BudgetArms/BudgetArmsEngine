#pragma once

#include <memory>

#include <SDL3/SDL_events.h>

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


        void BeginInput() const;
        void ProcessEvent(const SDL_Event& event) const;
        void ExecuteCommands() const;
        void ClearCommands() const;

        void AddKeyboardCommands(std::unique_ptr<Command> command, unsigned int key,
                                 InputManager::ButtonState activationState) const;

        [[nodiscard]] bool IsKeyUp(unsigned int key) const;
        [[nodiscard]] bool IsKeyDown(unsigned int key) const;
        [[nodiscard]] bool IsKeyPressed(unsigned int key) const;

    private:
        // Pimple implementation
        class Impl;
        std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };
    };
}


