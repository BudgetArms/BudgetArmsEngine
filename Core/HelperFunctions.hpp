#pragma once
#include <ctime>

#define FUNCTION_NAME __FUNCTION__

namespace bae
{
    // TODO: Fix random number generation by using mt19937 & high precision clock
    // TODO: Use LibSodium instead, it's really random, while mt19937 isn't
    inline int GetRandomNumber()
    {
        static bool bSeeded = false;
        if(!bSeeded)
        {
            std::srand(static_cast<unsigned int>(std::time(nullptr)));
            bSeeded = true;
        }

        return std::rand();
    }
}

