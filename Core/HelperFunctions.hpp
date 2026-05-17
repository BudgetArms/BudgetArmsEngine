#pragma once
#include <cstdlib>
#include <ctime>
#include <string_view>

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

    // This is the real original SDBM function
    // Not to be confused with the 'faster' version of the algorithm
    constexpr unsigned int HashSDBM(const std::string_view stringView, const unsigned int hash = 0)
    {
        if(stringView.empty())
        {
            return hash;
        }

        return HashSDBM(stringView.substr(1), hash * 65599u + static_cast<unsigned char>(stringView[0]));
    }
}

