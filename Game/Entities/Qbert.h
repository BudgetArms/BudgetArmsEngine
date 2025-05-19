#pragma once

#include <string>
#include <iostream>

#include "Core/GameObject.h"


namespace Game
{
    class HealthComponent;

    class Qbert : public bae::GameObject
    {
    public:
        Qbert(const std::string& name);
        ~Qbert() {};

        Qbert(const Qbert& other) = delete;
        Qbert(Qbert&& other) = delete;
        Qbert& operator=(const Qbert& other) = delete;
        Qbert& operator=(Qbert&& other) = delete;




    };

}


