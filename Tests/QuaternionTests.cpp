#include <catch.hpp>
#include "StdH.h"

TEST_CASE("Quaternion <-> euler")
{
	Quaternion a(1, 0, 0, 0);
	Vector3 result = a.ToEuler();

	Vector3 euler = Vector3(0, 0, 0);

	REQUIRE(Compare::Vectors(result, euler));

	// convert back from euler to quat
	Quaternion converted = Quaternion(euler);
	REQUIRE(Compare::Quaternions(a, converted));
}

TEST_CASE("Quaternion <-> matrix")
{
	Quaternion a(0.14145f, 0.404144f, 0.808287f, 0.404144f);

	// calculate
	Matrix3 result = a.ToRotationMatrix();

	// must be that matrix
	float arr[] = { -0.633319f, 0.538996f, 0.555329f , 0.76766f, 0.346671f, 0.538996f , 0.0980003f, 0.76766f, -0.633319f };
	Matrix3 m(9, arr);
	// must be transposed, engine uses transposed tranformation matrices
	m.Transpose();

	REQUIRE(Compare::Matrices(m, result));

	// convert back from matrix to quat
	Quaternion converted = Quaternion(m);
	REQUIRE(Compare::Quaternions(a, converted));
}

TEST_CASE("Quaternion <-> axis angle")
{
	Quaternion a(0.14145f, 0.404144f, 0.808287f, 0.404144f);

	Vector3 resultAxis;
	float resultAngle; // in radians
	a.ToAxisAngle(resultAxis, resultAngle);

	Vector3 axis(0.404144f, 0.808287f, 0.404144f);
	float angle = DEG2RAD(163.736f); // to radians

	REQUIRE(Compare::Vectors(resultAxis, axis, 0.01f));
	REQUIRE(Compare::Floats(resultAngle, angle));

	// convert back from axis-angle to quat
	Quaternion converted = Quaternion(axis, angle);
	REQUIRE(Compare::Quaternions(a, converted, 0.01f));
}

TEST_CASE("Quaternion inverse")
{
	Quaternion a(0.14145f, 0.404144f, 0.808287f, 0.404144f);
	Quaternion result = Quaternion::Inverse(a);

	Quaternion inversed = Quaternion(0.14145f, -0.404144f, -0.808286f, -0.404144f);

	REQUIRE(Compare::Quaternions(result, inversed));
}

TEST_CASE("Quaternion conjugate")
{
	Quaternion a(0.14145f, 0.404144f, 0.808287f, 0.404144f);
	Quaternion result = Quaternion::Conjugate(a);

	Quaternion conjugated = Quaternion(0.14145f, -0.404144f, -0.808286f, -0.404144f);

	REQUIRE(Compare::Quaternions(result, conjugated));
}
