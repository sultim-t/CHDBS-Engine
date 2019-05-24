#include "StdH.h"
#include <catch.hpp>
#include <Engine/DataStructures/LinkedList.h>

TEST_CASE("Linked list")
{
	LinkedList<int> list;
	REQUIRE(list.GetSize() == 0);

	// add
	list.Add(2);
	list.Add(4);
	list.Add(8);
	list.Add(16);
	REQUIRE(list.GetSize() == 4);

	// indices
	REQUIRE((list[0] == 2 && list[1] == 4 && list[2] == 8 && list[3] == 16));

	// assign
	list[2] = 32;

	// remove
	list.Remove(32);
	REQUIRE(list.GetSize() == 3);

	list.Remove(2);
	REQUIRE(list.GetSize() == 2);
	REQUIRE(*list.GetHead() == 4);

	// find
	REQUIRE(list.Find(16));

	// deallocate
	list.Delete();
	REQUIRE(list.GetSize() == 0);
}