#pragma once

#include <memory>


#include "Commands/Command.hpp"
#include "Managers/InputManager.hpp"


namespace bae
{
    class Controller final
    {
    public:
        explicit Controller(int controllerIndex);
        ~Controller();

        Controller(const Controller&)            = delete;
        Controller(Controller&&)                 = delete;
        Controller& operator=(const Controller&) = delete;
        Controller& operator=(Controller&&)      = delete;


        void ProcessInput() const;
        void ClearCommands() const;

        void AddControllerCommands(std::unique_ptr<Command>, unsigned int button,
                                   InputManager::ButtonState activationState) const;

        [[nodiscard]] bool IsButtonUp(unsigned int button) const;
        [[nodiscard]] bool IsButtonDown(unsigned int button) const;
        [[nodiscard]] bool IsButtonPressed(unsigned int button) const;

        [[nodiscard]] int GetControllerIndex() const { return m_ControllerIndex; }

    private:
        // Pimple implementation
        class Impl;
        std::unique_ptr<Impl> m_Pimpl{};

        int m_ControllerIndex;
    };
}
