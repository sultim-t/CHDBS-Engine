#include "StdH.h"
#include <catch.hpp>
#include <Engine/Math/Transform.h>

TEST_CASE("Transform traslation")
{
	Transform t;

	t.Translate(Vector3(1, 20, 300));
	REQUIRE(Compare::Vectors(t.GetPosition(), Vector3(1, 20, 300)));
	
	t.Translate(Vector3(3, 10, -300));
	REQUIRE(Compare::Vectors(t.GetPosition(), Vector3(4, 30, 0)));

	t.SetPosition(Vector3(-100, 0, 100));
	REQUIRE(Compare::Vectors(t.GetPosition(), Vector3(-100, 0, 100)));
}

TEST_CASE("Transform scale")
{
	Transform t;

	t.Scale(Vector3(10, 20, 30));
	REQUIRE(Compare::Vectors(t.GetScale(), Vector3(10, 20, 30)));

	t.SetScale(Vector3(-10, -20, 10));
	REQUIRE(Compare::Vectors(t.GetScale(), Vector3(-10, -20, 10)));
}

TEST_CASE("Transform rotation")
{
	Transform t;
	Quaternion q(0.7f, 0.7f, 0.7f, 0.7f);

	t.Rotate(q);
	REQUIRE(Compare::Quaternions(t.GetRotation(), q));

	REQUIRE(Compare::Vectors(t.GetEuler(), q.ToEuler()));
}

TEST_CASE("Transform front / right / up")
{
	Transform t;
	Quaternion q(0.381246f, 0.533745f, 0.533745f, 0.533745f);

	// rotate
	t.Rotate(q);
	
	Vector3 f = { 0.976f, 0.163f, -0.139f };
	Vector3 r = { -0.139f, 0.977f, 0.163f };
	Vector3 u = { 0.163f, -0.139f, 0.977f };

	REQUIRE(Compare::Vectors(t.GetForward(), f));
	REQUIRE(Compare::Vectors(t.GetRight(), r));
	REQUIRE(Compare::Vectors(t.GetUp(), u));

	// mustn't change
	t.Translate(Vector3(100, 200, 150));

	REQUIRE(Compare::Vectors(t.GetForward(), f));
	REQUIRE(Compare::Vectors(t.GetRight(), r));
	REQUIRE(Compare::Vectors(t.GetUp(), u));
}