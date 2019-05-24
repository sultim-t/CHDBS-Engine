#include "StdH.h"
#include <catch.hpp>
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/StaticStack.h>

TEST_CASE("Static array")
{
	StaticArray<int> a;
	REQUIRE(a.GetSize() == 0);

	a.Init(4);
	REQUIRE(a.GetSize() == 4);

	SECTION("Default values")
	{
		int d[] = { 0,0,0,0 };
		REQUIRE(Compare::Arrays(4, a.GetArray(), d));
	}

	SECTION("Copy")
	{
		StaticArray<int> b;
		b.Init(4);

		b[0] = 100;
		b[1] = 300;
		b[2] = 400;
		b[3] = 600;

		a.CopyFrom(b);

		REQUIRE(Compare::Arrays(4, a.GetArray(), b.GetArray()));
	}

	SECTION("Delete")
	{
		a.Delete();
		REQUIRE(a.GetSize() == 0);
	}
}

TEST_CASE("Static stack")
{
	StaticStack<int> a;
	REQUIRE(a.GetSize() == 0);

	a.Init(4);
	REQUIRE(a.GetSize() == 4);

	SECTION("Push / pop")
	{
		a.Push(12);
		a.Push(24);
		a.Push(36);

		// top index
		REQUIRE(a.GetTop() == 3);

		REQUIRE(a[0] == 12);
		REQUIRE(a[1] == 24);
		REQUIRE(a[2] == 36);

		// pop
		REQUIRE(a.Pop() == 36);
		REQUIRE(a.Pop() == 24);

		REQUIRE(a.GetTop() == 1);

		// clear
		a.Clear();
		REQUIRE(a.GetTop() == 0);
	}
}