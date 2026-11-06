#pragma once
/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EHeuristicFunctions.h: Supported Heuristic functions in the engine.
// Dependencies: math.h and algorithm
/*=============================================================================*/

#include <algorithm>
#include <cmath>

// to undefine windows macro's
#ifdef max
#undef max
#endif


namespace bae::Graphs::HeuristicFunctions
{
    // Manhattan distance
    [[maybe_unused]] static float Manhattan(const float x, const float y)
    {
        return x + y;
    }

    // Euclidean distance
    [[maybe_unused]] static float Euclidean(const float x, const float y)
    {
        return std::sqrtf(x * x + y * y);
    }

    // Sqrt Euclidean distance
    [[maybe_unused]] static float SqEuclidean(const float x, const float y)
    {
        return x * x + y * y;
    }

    // Octile distance
    [[maybe_unused]] static float Octile(const float x, const float y)
    {
        const float f = std::sqrtf(2) - 1; // std::sqrt is constexpr in C++26
        if(x < y)
        {
            return f * x + y;
        }

        return f * y + x;
    }

    //Chebyshev distance
    [[maybe_unused]] static float Chebyshev(const float x, const float y)
    {
        return std::max(x, y);
    }
}
