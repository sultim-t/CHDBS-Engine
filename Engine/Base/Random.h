#pragma once

class Random
{
public:
	// Get random bool value
	static bool GetBool();
	// Get random integer from range [0..max]

	static int GetInt(int max);
	// Get random integer from range [min..max]
	static int GetInt(int min, int max);
	// Get random float from range [0..1]

	static float GetFloat();
	// Get random float from range [0..max]
	static float GetFloat(float max);
	// Get random float from range [min..max]
	static float GetFloat(float min, float max);

	// Get random vector on a sphere with radius 1
	static Vector3 GetOnSphere();
	// Get random vector on a sphere with radius "r"
	static Vector3 GetOnSphere(float r);
	// Get random vector inside on a sphere with radius 1
	static Vector3 GetInsideSphere();
	// Get random vector inside a sphere with radius "r"
	static Vector3 GetInsideSphere(float r);

	// Get random vector inside a box
	static Vector3 GetInsideBox(const Vector3 &extent);
};