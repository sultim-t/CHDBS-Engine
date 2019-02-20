#include "Intersection.h"

bool Intersection::SegmentTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &v0, const Vector3 &v1, const Vector3 &v2)
{
	// normal vector of triangle
	Vector3 n = Vector3::Cross(v1 - v0, v2 - v0);

	float da = Vector3::Dot((a - v0), n);
	float db = Vector3::Dot((b - v0), n);

	// if both are above or below
	if ((da > 0 && db > 0) || (da < 0 && db < 0))
	{
		return false;
	}

	Vector3 x = (b * da - a * db) / (da - db);
	return true;
}

bool Intersection::TriangleTriangle(const Vector3 & u0, const Vector3 & u1, const Vector3 & u2, const Vector3 & v0, const Vector3 & v1, const Vector3 & v2)
{
	// https://web.stanford.edu/class/cs277/resources/papers/Moller1997b.pdf

	// 1.

	// plane equation of v triangle
	Vector3 n1 = Vector3::Cross(v1 - v0, v2 - v0);
	float d2 = -Vector3::Dot(n1, v0);

	// u verts into plane equation to compute signed distances
	float du[3];
	du[0] = Vector3::Dot(n1, u0) + d2;
	du[1] = Vector3::Dot(n1, u1) + d2;
	du[2] = Vector3::Dot(n1, u2) + d2;

	// if same sign on all or equal 0
	// then no intersection occurs
	float du01 = du[0] * du[1];
	float du02 = du[0] * du[2];
	if (du01 > 0.0f && du02 > 0.0f)
	{
		return false;
	}

	// 2. Same for another triangle
	
	// plane equation of u triangle
	Vector3 n2 = Vector3::Cross(u1 - u0, u2 - u0);
	float d1 = -Vector3::Dot(n2, u0);

	// v verts into plane equation to compute signed distances
	float dv[3];
	dv[0] = Vector3::Dot(n2, v0) + d1;
	dv[1] = Vector3::Dot(n2, v1) + d1;
	dv[2] = Vector3::Dot(n2, v2) + d1;

	// if same sign on all or equal 0
	// then no intersection occurs
	float dv01 = dv[0] * dv[1];
	float dv02 = dv[0] * dv[2];
	if (dv01 > 0.0f && dv02 > 0.0f)
	{
		return false;
	}
	
	// 3.

	// direction of intersection line
	Vector3 dir = Vector3::Cross(n1, n2);

	// get max component of dir
	float max = ABS(dir[0]);
	int index = 0;
	float bb = ABS(dir[1]);
	float cc = ABS(dir[2]);
	if (bb > max)
	{
		max = bb; index = 1;
	}
	if (cc > max) 
	{
		max = cc; index = 2;
	}

	// compute interval for triangle 1
	float a, b, c, x0, x1;
	if (!ComputeIntervals(v0[index], v1[index], v2[index], dv[0], dv[1], dv[2], dv01, dv02, a, b, c, x0, x1))
	{
		return CoplanarTriTri(n1, v0, v1, v2, u0, u1, u2);
	}

	// compute interval for triangle 2
	float d, e, f, y0, y1;
	if (!ComputeIntervals(u0[index], u1[index], u2[index], du[0], du[1], du[2], du01, du02, d, e, f, y0, y1))
	{
		return CoplanarTriTri(n1, v0, v1, v2, u0, u1, u2);
	}

	float xx, yy, xxyy, tmp;
	float isect1[2], isect2[2];

	xx = x0*x1;
	yy = y0*y1;
	xxyy = xx*yy;

	tmp = a*xxyy;
	isect1[0] = tmp + b*x1*yy;
	isect1[1] = tmp + c*x0*yy;

	tmp = d*xxyy;
	isect2[0] = tmp + e*xx*y1;
	isect2[1] = tmp + f*xx*y0;

	if (isect1[0] > isect1[1])
	{
		float c;
		c = isect1[0];
		isect1[0] = isect1[1];
		isect1[1] = c;
	}

	if (isect2[0] > isect2[1])
	{
		float c;
		c = isect2[0];
		isect2[0] = isect2[1];
		isect2[1] = c;
	}

	if (isect1[1]<isect2[0] || isect2[1]<isect1[0]) return 0;
	return 1;
}

bool Intersection::ComputeIntervals(const float &VV0, const float &VV1, const float &VV2, const float &D0, const float &D1, const float &D2, const float &D0D1, const float &D0D2, float &A, float &B, float &C, float &X0, float &X1)
{
	if (D0D1 > 0.0f)
	{
		// here we know that D0D2<=0.0 
		// that is D0, D1 are on the same side, D2 on the other or on the plane 
		A = VV2; B = (VV0 - VV2)*D2; C = (VV1 - VV2)*D2; X0 = D2 - D0; X1 = D2 - D1;
		return true;
	}
	else if (D0D2 > 0.0f)
	{
		// here we know that d0d1<=0.0 
		A = VV1; B = (VV0 - VV1)*D1; C = (VV2 - VV1)*D1; X0 = D1 - D0; X1 = D1 - D2;
		return true;
	}
	else if (D1*D2 > 0.0f || D0 != 0.0f)
	{
		// here we know that d0d1<=0.0 or that D0!=0.0 
		A = VV0; B = (VV1 - VV0)*D0; C = (VV2 - VV0)*D0; X0 = D0 - D1; X1 = D0 - D2;
		return true;
	}
	else if (D1 != 0.0f)
	{
		A = VV1; B = (VV0 - VV1)*D1; C = (VV2 - VV1)*D1; X0 = D1 - D0; X1 = D1 - D2;
		return true;
	}
	else if (D2 != 0.0f)
	{
		A = VV2; B = (VV0 - VV2)*D2; C = (VV1 - VV2)*D2; X0 = D2 - D0; X1 = D2 - D1;
		return true;
	}
	else
	{
		return false;
	}
}

bool Intersection::CoplanarTriTri(const Vector3 &N, const Vector3 &V0, const Vector3 &V1, const Vector3 &V2, const Vector3 &U0, const Vector3 &U1, const Vector3 &U2)
{
	float A[3];
	short i0, i1;
	// first project onto an axis-aligned plane, that maximizes the area 
	// of the triangles, compute indices: i0,i1. 
	A[0] = ABS(N[0]);
	A[1] = ABS(N[1]);
	A[2] = ABS(N[2]);
	if (A[0]>A[1])
	{
		if (A[0]>A[2])
		{
			i0 = 1;      // A[0] is greatest 
			i1 = 2;
		}
		else
		{
			i0 = 0;      // A[2] is greatest 
			i1 = 1;
		}
	}
	else   // A[0]<=A[1] 
	{
		if (A[2]>A[1])
		{
			i0 = 0;      // A[2] is greatest 
			i1 = 1;
		}
		else
		{
			i0 = 0;      // A[1] is greatest 
			i1 = 2;
		}
	}

	// test all edges of triangle 1 against the edges of triangle 2 
	if (EdgeAgainstTriEdges(V0, V1, U0, U1, U2, i0, i1))
	{
		return true;
	}

	if (EdgeAgainstTriEdges(V1, V2, U0, U1, U2, i0, i1))
	{
		return true;
	}
	
	if (EdgeAgainstTriEdges(V2, V0, U0, U1, U2, i0, i1))
	{
		return true;
	}

	// finally, test if tri1 is totally contained in tri2 or vice versa 
	if (PointInTri(V0, U0, U1, U2, i0, i1))
	{
		return true;
	}

	if (PointInTri(U0, V0, V1, V2, i0, i1))
	{
		return true;
	}

	return false;
}

bool Intersection::PointInTri(const Vector3 & V0, const Vector3 & U0, const Vector3 & U1, const Vector3 & U2, const int i0, const int i1)
{
	float a, b, c, d0, d1, d2;
	// is T1 completly inside T2? 
	// check if V0 is inside tri(U0,U1,U2) 
	a = U1[i1] - U0[i1];
	b = -(U1[i0] - U0[i0]);
	c = -a*U0[i0] - b*U0[i1];
	d0 = a*V0[i0] + b*V0[i1] + c;

	a = U2[i1] - U1[i1];
	b = -(U2[i0] - U1[i0]);
	c = -a*U1[i0] - b*U1[i1];
	d1 = a*V0[i0] + b*V0[i1] + c;

	a = U0[i1] - U2[i1];
	b = -(U0[i0] - U2[i0]);
	c = -a*U2[i0] - b*U2[i1];
	d2 = a*V0[i0] + b*V0[i1] + c;
	if (d0*d1 > 0.0)
	{
		if (d0*d2 > 0.0) return true;
	}

	return false;
}

// gpu gems III
bool Intersection::EdgeEdgeTest(const Vector3 & V0, const Vector3 & U0, const Vector3 & U1, const int i0, const int i1,
	float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &e, float &d, float &f)
{
	Bx = U0[i0] - U1[i0];
	By = U0[i1] - U1[i1];
	Cx = V0[i0] - U0[i0];
	Cy = V0[i1] - U0[i1];
	f = Ay*Bx - Ax*By;
	d = By*Cx - Bx*Cy;
	if ((f > 0 && d >= 0 && d <= f) || (f < 0 && d <= 0 && d >= f))
	{
		e = Ax*Cy - Ay*Cx;
		if (f > 0)
		{
			if (e >= 0 && e <= f) return 1;
		}
		else
		{
			if (e <= 0 && e >= f) return 1;
		}
	}
}

bool Intersection::EdgeAgainstTriEdges(const Vector3 & V0, const Vector3 & V1, const Vector3 & U0, const Vector3 & U1, const Vector3 & U2, const int i0, const int i1)
{
	float Ax, Ay, Bx, By, Cx, Cy, e, d, f;
	Ax = V1[i0] - V0[i0];
	Ay = V1[i1] - V0[i1];
	// test edge U0,U1 against V0,V1 
	if (EdgeEdgeTest(V0, U0, U1, i0, i1, Ax, Ay, Bx, By, Cx, Cy, e, d, f))
	{
		return true;
	}

	// test edge U1,U2 against V0,V1 
	if (EdgeEdgeTest(V0, U1, U2, i0, i1, Ax, Ay, Bx, By, Cx, Cy, e, d, f))
	{
		return true;
	}

	// test edge U2,U1 against V0,V1 
	if (EdgeEdgeTest(V0, U2, U0, i0, i1, Ax, Ay, Bx, By, Cx, Cy, e, d, f))
	{
		return true;
	}

	return false;
}