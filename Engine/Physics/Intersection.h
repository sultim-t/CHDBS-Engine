#pragma once
#include <Engine/Math/Vector.h>

class Intersection
{
private:
	static bool ComputeIntervals(const float &VV0, const float &VV1, const float &VV2, const float &D0, const float &D1, const float &D2, const float &D0D1, const float &D0D2, float &A, float &B, float &C, float &X0, float &X1);
	static bool CoplanarTriTri(const Vector3 &N, const Vector3 &V0, const Vector3 &V1, const Vector3 &V2, const Vector3 &U0, const Vector3 &U1, const Vector3 &U2);
	static bool PointInTri(const Vector3 &V0, const Vector3 &U0, const Vector3 &U1, const Vector3 &U2, const int i0, const int i1);
	static bool EdgeEdgeTest(const Vector3 &V0, const Vector3 &U0, const Vector3 &U1, const int i0, const int i1, float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &e, float &d, float &f);
	static bool EdgeAgainstTriEdges(const Vector3 &V0, const Vector3 &V1, const Vector3 &U0, const Vector3 &U1, const Vector3 &U2, const int i0, const int i1);;

public:
	static bool SegmentTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
	static bool TriangleTriangle(const Vector3 &u0, const Vector3 &u1, const Vector3 &u2, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
};