#pragma once
#include "Singletons/Singleton.h"


namespace bae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool ProcessInput();
    };

}

