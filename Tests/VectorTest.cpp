#include <catch.hpp>
#include <Engine/Base/Assert.h>
#include <Engine/Base/Typedef.h>
#include <Engine/Math/Vector.h>
//
//bool Compare(float a, float b, float eps);
//bool Compare(float a, float b, float eps)
//{
//	return Abs(a - b) < eps;
//}

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

//TEST_CASE("Vector3 length")
//{
//	Vector3 a(2, 3, 4);
//	float length = 5.0f;
//
//	bool c = Compare(a.Length(), length, 0.001f);
//	REQUIRE(c);
//}