#pragma once

#include <memory>
#include <vector>

#include <Windows.h>
#include <Xinput.h>

#include "Commands/Command.h"
#include "Managers/InputManager.h"


namespace bae
{

    class Controller
    {
    public:
        explicit Controller(int controllerIndex);
        ~Controller();

        Controller(const Controller& other) = delete;
        Controller(Controller&& other) = delete;
        Controller& operator=(const Controller& other) = delete;
        Controller& operator=(Controller&& other) = delete;


        void ProcessInput();
        void AddControllerCommands(std::unique_ptr<Command>, unsigned int button, InputManager::ButtonState activationState);


        bool IsButtonPressed(unsigned int button) const;

        int GetControllerIndex() const { return m_ControllerIndex; };


    private:

        // Pimple implementation
        class Impl;
        std::unique_ptr<Impl> m_Pimpl{ std::make_unique<Impl>() };

        int m_ControllerIndex;

    protected:


    };

}