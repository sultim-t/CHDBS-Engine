#include <catch.hpp>
#include "StdH.h"

TEST_CASE("Vector3 elements")
{
	VectorI3 a(1, 2, 3);

	REQUIRE(a[0] == 1);
	
	a[1] = 6;
	REQUIRE(a[1] == 6);
}

TEST_CASE("Vector3 sum")
{
	Vector3 a(1, 2, 3);
	Vector3 b(3, 5, 6);
	Vector3 sum(4, 7, 9);

	REQUIRE(sum == a + b);
}

TEST_CASE("Vector3 dot")
{
	Vector3 a(1, 2, 3);
	Vector3 b(3, 5, 6);
	float result = 31;

	REQUIRE(result == Vector3::Dot(a, b));
}

TEST_CASE("Vector3 cross product")
{
	Vector3 a(2, 3, 4);
	Vector3 b(5, 6, 7);
	Vector3 result(-3, 6, -3);

	REQUIRE(result == Vector3::Cross(a, b));
}

TEST_CASE("Vector3 length")
{
	Vector3 a(2, 3, 4);
	float length = 5.385f;

	REQUIRE(Compare::Floats(a.Length(), length));
}

TEST_CASE("Vector3 normalization")
{
	Vector3 a(2, 3, 4);
	a.Normalize();

	REQUIRE(Compare::Floats(a.Length(), 1));
}