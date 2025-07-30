#pragma once


#define GetFunctionName() __FUNCTION__

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


