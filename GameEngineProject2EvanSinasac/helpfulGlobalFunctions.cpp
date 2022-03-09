#include "helpfulGlobalFunctions.h"

#include <iostream>


void g_coutAVec3(glm::vec3 theVec3, bool bNewLine /*=false*/)
{
	std::cout << "  " << theVec3.x << ", " << theVec3.y << ", " << theVec3.z << "   ";
	if (bNewLine)
	{
		std::cout << std::endl;
	}
	return;
}