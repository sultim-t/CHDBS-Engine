#include "Triangle.h"

Triangle::Triangle(const Vector3 & a, const Vector3 & b, const Vector3 & c)
	: A(a), B(b), C(c) { }

Vector3 Triangle::GetClosestPoint(const Vector3 & p, const Vector3 & a, const Vector3 & b, const Vector3 & c)
{
	// Check if P in vertex region outside A
	Vector3 ab = b - a;
	Vector3 ac = c - a;
	Vector3 ap = p - a;
	float d1 = Vector3::Dot(ab, ap);
	float d2 = Vector3::Dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) 
	{
		return a; // barycentric coordinates (1,0,0)
	}

	// Check if P in vertex region outside B
	Vector3 bp = p - b;
	float d3 = Vector3::Dot(ab, bp);
	float d4 = Vector3::Dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3)
	{
		return b; // barycentric coordinates (0,1,0)
	}
										  
	// Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) 
	{
		float v = d1 / (d1 - d3);
		return a + ab * v; // barycentric coordinates (1-v,v,0)
	}
	
	// Check if P in vertex region outside C
	Vector3 cp = p - c;
	float d5 = Vector3::Dot(ab, cp);
	float d6 = Vector3::Dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		// barycentric coordinates (0,0,1)
		return c;
	}	
	
	// Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5*d2 - d1*d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return a + a * w; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3*d6 - d5*d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + (c - b) * w; // barycentric coordinates (0,1-w,w)
	}

	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;

	return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f-v-w
}

// slow finding of a point
//{
//	Vector3 ab = b - a;
//	Vector3 ac = c - a;
//	Vector3 bc = c - b;
//
//	float snom = Vector3::Dot(p - a, ab);
//	float sdenom = Vector3::Dot(p - b, a - b);
//
//	float tnom = Vector3::Dot(p - a, ac);
//	float tdenom = Vector3::Dot(p - c, a - c);
//
//	if (snom <= 0.0f && tnom <= 0.0f)
//	{
//		return a;
//	}
//
//	float unom = Vector3::Dot(p - b, bc);
//	float udenom = Vector3::Dot(p - c, b - c);
//
//	if (sdenom <= 0.0f && unom <= 0.0f)
//	{
//		return b;
//	}
//
//	if (tdenom <= 0.0f && udenom <= 0.0f)
//	{
//		return c;
//	}
//
//	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
//	Vector3 n = Vector3::Cross(b - a, c - a);
//	float vc = Vector3::Dot(n, Vector3::Cross(a - p, b - p));
//
//	// If P outside AB and within feature region of AB
//	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
//	{
//		// return projection of P on AB
//		return a + ab * snom / (snom + sdenom);
//	}
//
//	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
//	float va = Vector3::Dot(n, Vector3::Cross(b - p, c - p));
//	// If P outside BC and within feature region of BC
//	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
//	{
//		// return projection of P onto BC
//		return b + bc * unom / (unom + udenom);
//	}
//
//	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
//	float vb = Vector3::Dot(n, Vector3::Cross(c - p, a - p));
//
//	// If P outside CA and within feature region of CA
//	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
//	{
//		// return projection of P onto CA
//		return a + ac * tnom / (tnom + tdenom);
//	}
//	
//	// P must project inside face region. Compute Q using barycentric coordinates
//	float u = va / (va + vb + vc);
//	float v = vb / (va + vb + vc);
//	float w = 1.0f - u - v;
//
//	return a * u + b * v + c * w;
//}
