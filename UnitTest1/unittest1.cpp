#include "stdafx.h"
#include "CppUnitTest.h"
#include "../TANK/Ground.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(sss)
		{
			SDL_Point p = pixelToGroundPoint({0, 0});
		}

	};
}