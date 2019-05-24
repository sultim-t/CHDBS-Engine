#include "StdH.h"
#include <catch.hpp>
#include <Engine/DataStructures/DynamicArray.h>

TEST_CASE("Dynamic array")
{
	DynamicArray<int> a;
	REQUIRE(a.GetSize() == 0);

	// allocate memory for 4 elements
	a.Init(4);

	// must be no elements (only row memory)
	REQUIRE(a.GetSize() == 0);

	SECTION("Push / pop")
	{
		a.Push(12);
		a.Push(24);
		a.Push(36);

		// top index
		REQUIRE(a.GetSize() == 3);

		REQUIRE(a[0] == 12);
		REQUIRE(a[1] == 24);
		REQUIRE(a[2] == 36);

		// pop
		REQUIRE(a.Pop() == 36);
		REQUIRE(a.Pop() == 24);

		REQUIRE(a.GetSize() == 1);

		// clear
		a.Clear();
		REQUIRE(a.GetSize() == 0);
	}

	SECTION("Push more than allocated")
	{
		for (int i = 0; i < 10; i++)
		{
			a.Push(i);
		}

		REQUIRE(a.GetSize() == 10);
	}

	SECTION("Delete")
	{
		a.Delete();
		REQUIRE(a.GetSize() == 0);
	}
}