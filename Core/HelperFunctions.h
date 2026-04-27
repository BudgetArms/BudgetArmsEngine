#pragma once

#include <time.h>


#define GetFunctionName() __FUNCTION__

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


