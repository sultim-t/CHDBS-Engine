#include "StdH.h"
#include <catch.hpp>
#include <Engine/Math/Matrix.h>

TEST_CASE("Matrix constructors")
{
	float id[] =
	{	1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1		};
	float af[] =
	{ 8,1,4,3,
		5,9,2,6,
		10,7,1,5,
		7,8,4,5		};
	float sf[] =
	{	3,3,3,3,
		3,3,3,3,
		3,3,3,3,
		3,3,3,3		};


	Matrix4 identity(1.0f, true);
	Matrix4 s(3.0f);
	Matrix4 a(16, af);

	REQUIRE(Compare::Arrays(16, id, identity.ToArray()));
	REQUIRE(Compare::Arrays(16, sf, s.ToArray()));
	REQUIRE(Compare::Arrays(16, af, a.ToArray()));
}

TEST_CASE("Matrix multiplication")
{
	float af[] = 
	{	8,1,4,3,
		5,9,2,6,
		10,7,1,5,
		7,8,4,5		};

	// identity matrix
	Matrix4 identity(1.0f, true);
	Matrix4 a(16, af);
	
	float a2f[] =
	{	130,69,50,65,
		147,148,64,109,
		160,120,75,102,
		171,147,68,114		};

	Matrix4 aSqr(16, a2f);

	REQUIRE(Compare::Matrices(a * identity, a));
	REQUIRE(Compare::Matrices(a * a, aSqr));
}

TEST_CASE("Transposed matrix")
{
	float af[] =
	{	6,4,8,13,
		7,1,9,5,
		12,6,11,8,
		9,16,10,0	};
	float atf[] =
	{	6,7,12,9,
		4,1,6,16,
		8,9,11,10,
		13,5,8,0 };

	Matrix4 a(16, af);
	Matrix4 transposed(16, atf);

	a.Transpose();

	REQUIRE(Compare::Matrices(transposed, a));
}

TEST_CASE("Matrix determinant")
{
	float af[] =
	{	1,2,1,3,
		2,2,1,0,
		3,1,2,1,
		0,1,2,1 };

	Matrix4 a(16, af);

	REQUIRE(a.GetDeterminant() == 27);
}

TEST_CASE("Inverse matrix")
{
	float af[] =
	{	1,2,1,3,  
		2,2,1,0,  
		3,1,2,1,  
		0,1,2,1	};

	float atf[] =
	{	0.0f, 0.0f, 1.0f / 3.0f, -1.0f / 3.0f,
		1.0f / 9.0f, 5.0f / 9.0f, -11.0f / 27.0f, 2 / 27.0f,
		-2.0f / 9.0f, -1.0f / 9.0f, 4.0f / 27.0f, 14.0f / 27.0f,
		1.0f / 3.0f, -1.0f / 3.0f, 1.0f / 9.0f, -1.0f / 9.0f };

	Matrix4 a(16, af);
	Matrix4 inversed(16, atf);

	a.Inverse();

	REQUIRE(Compare::Matrices(inversed, a));
}

TEST_CASE("Matrix indices")
{
	float af[] =
	{	8,1,4,3,
		5,9,2,6,
		10,7,1,5,
		7,8,4,5		};

	Matrix4 a(16, af);

	// elements
	REQUIRE((a(0, 0) == 8 && a(1, 0) == 5 && a(3, 3) == 5));
	// matrix vectors
	REQUIRE(Compare::Vectors(a[0], Vector4(8, 1, 4, 3)));

	// assign
	a(3, 0) = 20;
	a(3, 3) = 30;

	Vector4 newRow(40, 41, 42, 43);
	a.SetRow(0, newRow);

	// elements
	REQUIRE((a(3, 0) == 20 && a(3, 3) == 30));
	// matrix vectors
	REQUIRE(Compare::Vectors(a[0], newRow));
}