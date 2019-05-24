#include "StdH.h"
#include <catch.hpp>
#include <Engine/Base/String.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>

TEST_CASE("String construstors")
{
	String a;
	REQUIRE(a == "");

	String b("Test string");
	REQUIRE(b == "Test string");

	String c(b);
	REQUIRE(c == "Test string");

	c.Clear();
	REQUIRE(c == "");
}

TEST_CASE("String indices")
{
	String a = "QWERTY";
	REQUIRE((a[0] == 'Q' && a[3] == 'R'));

	a[0] = 'C';
	REQUIRE(a[0] == 'C');
}

TEST_CASE("String assignment / comparison")
{
	String a = "Test string";
	REQUIRE(a == "Test string");

	String b = a;
	b[0] = 'A';

	REQUIRE(b[0] != a[0]);
	REQUIRE(b != a);
}

TEST_CASE("String concatenation")
{
	String a = "First-";
	a += "Second";
	REQUIRE(a == "First-Second");

	String b = String("String ") + "conca" + "tenation";
	REQUIRE(b == "String concatenation");
}

TEST_CASE("String length")
{
	String a = "String length";
	REQUIRE(a.Length() == 13);
}

TEST_CASE("String convert")
{
	String a = "1";
	REQUIRE(a.ToBool() == true);
	REQUIRE(a.ToInt() == 1);

	a = "0";
	REQUIRE(a.ToBool() == false);

	a = "2334";
	REQUIRE(a.ToInt() == 2334);

	a = "120.1568";
	REQUIRE(a.ToFloat() == 120.1568f);

	a = "1.5 40.3 5000";
	REQUIRE(a.ToVector3() == Vector3(1.5f, 40.3f, 5000));

	a = "0.5 0.6 0.7 0.8";
	REQUIRE(a.ToQuaternion() == Quaternion(0.5f, 0.6f, 0.7f, 0.8f));
}