#pragma once

#include <string>
#include <source_location>

#include <iostream>


#define GetFunctionName() __FUNCTION__

/*
// this has too many symbols and parsing it would be hell bc compiles don't output the same
// code, while the good old macro works perfectly
inline std::string GetFunctionName()
{
	return std::source_location::current().function_name();
}
*/

// this didn't work in the beginning but somehow, now it does, ....
// I am a magician
inline int GetRandomNumber()
{
	static bool bSeeded = false;
	if (!bSeeded)
	{
		std::srand(static_cast<unsigned int>(std::time(NULL)));
		bSeeded = true;
	}

	return std::rand();
}
