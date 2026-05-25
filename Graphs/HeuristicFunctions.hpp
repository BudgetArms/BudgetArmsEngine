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


namespace bae::Graphs::HeuristicFunctions
{
    // Manhattan distance
    [[maybe_unused]] static float Manhattan(float x, float y)
    {
        return static_cast<float>(x + y);
    }

    // Euclidean distance
    [[maybe_unused]] static float Euclidean(float x, float y)
    {
        return std::sqrtf(x * x + y * y);
    }

    // Sqrt Euclidean distance
    [[maybe_unused]] static float SqEuclidean(float x, float y)
    {
        return x * x + y * y;
    }

    // Octile distance
    [[maybe_unused]] static float Octile(float x, float y)
    {
        //const float f = 0.414213562373095048801f; // == sqrt(2) - 1;
        const float f = std::sqrtf(2) - 1; // std::sqrt is constexpr in C++26
        return (x < y) ? f * x + y : f * y + x;
    }

    //Chebyshev distance
    [[maybe_unused]] static float Chebyshev(float x, float y)
    {
        return std::max(x, y);
    }
}


