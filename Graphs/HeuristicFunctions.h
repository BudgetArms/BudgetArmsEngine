#pragma once
/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// EHeuristicFunctions.h: Supported Heuristic functions in the engine.
// Dependencies: math.h and algorithm
/*=============================================================================*/


#include <cmath>
#include <algorithm>


namespace bae::Graphs::HeuristicFunctions
{
#pragma warning(push)
#pragma warning(disable : 4505)
	//Manhattan distance
	static float Manhattan(float x, float y)
	{
		return float(x + y);
	}

	//Euclidean distance
	static float Euclidean(float x, float y)
	{
		return float(std::sqrtf(x * x + y * y));
	}

	//Sqrt Euclidean distance
	static float SqEuclidean(float x, float y)
	{
		return float(x * x + y * y);
	}

	//Octile distance
	static float Octile(float x, float y)
	{
		//const float f = 0.414213562373095048801f; // == sqrt(2) - 1;
		const float f = std::sqrtf(2) - 1; // std::sqrt is constexpr in C++26
		return float((x < y) ? f * x + y : f * y + x);
	}

	//Chebyshev distance
	static float Chebyshev(float x, float y)
	{
		return std::max(x, y);
	}
#pragma warning(pop)


}


