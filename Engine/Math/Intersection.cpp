#include "Intersection.h"
#include "AABB.h"
#include "Frustum.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/ResourceManager/MeshResource.h>

#include <limits>

#pragma region AABBTriangleTests

#define X 0
#define Y 1
#define Z 2

#define AXISTEST_X01(a, b, fa, fb)			   \
p0 = a*v0[Y] - b*v0[Z];			       	   \
p2 = a*v2[Y] - b*v2[Z];			       	   \
if (p0<p2) { min = p0; max = p2; } else { min = p2; max = p0; } \
rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
if (min>rad || max<-rad) return false;

#define AXISTEST_X2(a, b, fa, fb)			   \
p0 = a*v0[Y] - b*v0[Z];			           \
p1 = a*v1[Y] - b*v1[Z];			       	   \
if (p0<p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
if (min>rad || max<-rad) return false;

#define AXISTEST_Y02(a, b, fa, fb)			   \
p0 = -a*v0[X] + b*v0[Z];		      	   \
p2 = -a*v2[X] + b*v2[Z];	       	       	   \
if (p0<p2) { min = p0; max = p2; } else { min = p2; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
if (min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
p0 = -a*v0[X] + b*v0[Z];		      	   \
p1 = -a*v1[X] + b*v1[Z];	     	       	   \
if (p0<p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
if (min>rad || max<-rad) return false;

#define AXISTEST_Z12(a, b, fa, fb)			   \
p1 = a*v1[X] - b*v1[Y];			           \
p2 = a*v2[X] - b*v2[Y];			       	   \
if (p2<p1) { min = p2; max = p1; } else { min = p1; max = p2; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
if (min>rad || max<-rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)			   \
p0 = a*v0[X] - b*v0[Y];				   \
p1 = a*v1[X] - b*v1[Y];			           \
if (p0<p1) { min = p0; max = p1; } else { min = p1; max = p0; } \
rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
if (min>rad || max<-rad) return false;

#define FINDMINMAX(x0,x1,x2,min,max) \
min = max = x0;   \
if (x1<min) min = x1; \
if (x1>max) max = x1; \
if (x2<min) min = x2; \
if (x2>max) max = x2;

#pragma endregion


bool Intersection::SphereSphere(const Sphere & s1, const Sphere & s2)
{
	float d = Vector3::DistanceSqr(s1.GetCenter(), s2.GetCenter());
	float radiusSum = s1.GetRadius() + s2.GetRadius();
	
	return d <= radiusSum * radiusSum;
}

bool Intersection::SphereSphere(const Sphere & s1, const Sphere & s2, Vector3 & point, Vector3 & normal)
{
	Vector3 delta = s2.GetCenter() - s1.GetCenter();

	float d = delta.LengthSqr();
	float radiusSum = s1.GetRadius() + s2.GetRadius();

	// if intersecting, calculate normal and position
	if (d <= radiusSum * radiusSum)
	{
		normal = delta / Sqrt(d);
		point = s1.GetCenter() + normal * s1.GetRadius();

		return true;
	}

	return false;
}

bool Intersection::SpherePlane(const Sphere & s, const Plane & p)
{
	float distToCenter = p.PlaneDot(s.GetCenter());
	return distToCenter <= s.GetRadius();
}

bool Intersection::TriangleSphere(const Triangle &t, const Sphere &s, Vector3 &point)
{
	// closest point
	point = t.GetClosestPoint(s.GetCenter());

	// distance from closest point on triangle to center
	Vector3 v = point - s.GetCenter();
	return v.LengthSqr() <= s.GetRadius() * s.GetRadius();
}

bool Intersection::TriangleAABB(const Triangle & t, const AABB & aabb)
{
	const Vector3 boxcenter = aabb.GetCenter();
	const Vector3 boxhalfsize = aabb.GetExtent();

	// https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/pubs/tribox.pdf

	Vector3 v0, v1, v2;
	float min, max, p0, p1, p2, rad, fex, fey, fez;
	Vector3 normal, e0, e1, e2;

	v0 = t.A - boxcenter;
	v1 = t.B - boxcenter;
	v2 = t.C - boxcenter;

	e0 = v1 - v0;
	e1 = v2 - v1;
	e2 = v0 - v2;

	// 9 tests
	fex = std::abs(e0[0]);
	fey = std::abs(e0[1]);
	fez = std::abs(e0[2]);
	AXISTEST_X01(e0[Z], e0[Y], fez, fey);
	AXISTEST_Y02(e0[Z], e0[X], fez, fex);
	AXISTEST_Z12(e0[Y], e0[X], fey, fex);

	fex = std::abs(e1[0]);
	fey = std::abs(e1[1]);
	fez = std::abs(e1[2]);
	AXISTEST_X01(e1[Z], e1[Y], fez, fey);
	AXISTEST_Y02(e1[Z], e1[X], fez, fex);
	AXISTEST_Z0(e1[Y], e1[X], fey, fex);

	fex = std::abs(e2[0]);
	fey = std::abs(e2[1]);
	fez = std::abs(e2[2]);
	AXISTEST_X2(e2[Z], e2[Y], fez, fey);
	AXISTEST_Y1(e2[Z], e2[X], fez, fex);
	AXISTEST_Z12(e2[Y], e2[X], fey, fex);

	// 3 tests: x, y, z directions
	FINDMINMAX(v0[X], v1[X], v2[X], min, max);
	if (min > boxhalfsize[X] || max < -boxhalfsize[X])
	{
		return false;
	}

	FINDMINMAX(v0[Y], v1[Y], v2[Y], min, max);
	if (min > boxhalfsize[Y] || max < -boxhalfsize[Y])
	{
		return false;
	}

	FINDMINMAX(v0[Z], v1[Z], v2[Z], min, max);
	if (min > boxhalfsize[Z] || max < -boxhalfsize[Z])
	{
		return false;
	}

	// test box and triangle plane
	normal = Vector3::Cross(e0, e1);
	
	if (!PlaneBoxOverlap(normal, v0, boxhalfsize))
	{
		return false;
	}

	return true;
}

bool Intersection::TriangleAABB(const Triangle & t, const AABB & aabb, Vector3 & point, Vector3 & normal)
{
	normal = t.GetNormal();
	point = t.GetClosestPoint(aabb.GetCenter());

	return TriangleAABB(t, aabb);
}

bool Intersection::TrianglePlane(const Triangle & t, const Plane & plane, Vector3 &p0, Vector3 &p1)
{
	// debug all references/pointers

	Vector3* points[] = { &p0, &p1 };
	int last = 0;

	if (SegmentPlane(t.A, t.B, plane, *points[last]))
	{
		last++;
	}

	if (SegmentPlane(t.B, t.C, plane, *points[last]))
	{
		last++;

		if (last == 2)
		{
			return true;
		}
	}

	if (SegmentPlane(t.C, t.A, plane, *points[last]))
	{
		last++;

		if (last == 2)
		{
			return true;
		}
	}

	return false;
}

bool Intersection::MeshSphere(const StaticArray<Triangle> &triangles, const Sphere & s, Vector3 & point, Vector3 &normal)
{
	UINT size = triangles.GetSize();

	for (UINT i = 0; i < size; i++)
	{
		if (TriangleSphere(triangles[i], s, point))
		{
			normal = triangles[i].GetNormal();
			return true;
		}
	}

	return false;
}

bool Intersection::MeshAABB(const StaticArray<Triangle> &triangles, const AABB & aabb)
{
	UINT size = triangles.GetSize();

	for (UINT i = 0; i < size; i++)
	{
		if (TriangleAABB(triangles[i], aabb))
		{
			return true;
		}
	}

	return false;
}

bool Intersection::MeshAABB(const StaticArray<Triangle>& triangles, const AABB & aabb, Vector3 & point, Vector3 & normal)
{
	UINT size = triangles.GetSize();

	for (UINT i = 0; i < size; i++)
	{
		const Triangle &t = triangles[i];

		if (TriangleAABB(t, aabb))
		{
			normal = t.GetNormal();
			point = t.GetClosestPoint(aabb.GetCenter());

			return true;
		}
	}

	return false;
}

bool Intersection::MeshPlane(const StaticArray<Triangle> &triangles, const Plane & p, Vector3 & start, Vector3 & end)
{
	UINT size = triangles.GetSize();

	for (UINT i = 0; i < size; i++)
	{
		if (TrianglePlane(triangles[i], p, start, end))
		{
			return true;
		}
	}

	return false;
}

bool Intersection::MeshRay(const StaticArray<Triangle> &triangles, const Ray & ray, Vector3 & worldPoint, Vector3 & normal)
{
	UINT size = triangles.GetSize();
	Vector3 barycentric;

	for (UINT i = 0; i < size; i++)
	{
		if (RayTriangle(ray, triangles[i], barycentric))
		{
			worldPoint = triangles[i].GetCartesian(barycentric);
			normal = triangles[i].GetNormal().GetNormalized();
			return true;
		}
	}

	return false;
}

bool Intersection::MeshSegment(const StaticArray<Triangle> &triangles, const Vector3 & start, const Vector3 & end, Vector3 & worldPoint)
{
	UINT size = triangles.GetSize();
	Vector3 barycentric;

	for (UINT i = 0; i < size; i++)
	{
		if (SegmentTriangle(start, end, triangles[i], barycentric))
		{
			worldPoint = triangles[i].GetCartesian(barycentric);
			return true;
		}
	}

	return false;
}

bool Intersection::PlaneBoxOverlap(const Vector3 & normal, const Vector3 & vert, const Vector3 & maxbox)
{
	Vector3 ovmin, ovmax;
	float ov;

	for (int oq = X; oq <= Z; oq++)
	{
		ov = vert[oq];

		if (normal[oq] > 0.0f)
		{
			ovmin[oq] = -maxbox[oq] - ov;
			ovmax[oq] = maxbox[oq] - ov;
		}
		else
		{
			ovmin[oq] = maxbox[oq] - ov;
			ovmax[oq] = -maxbox[oq] - ov;
		}
	}

	if (Vector3::Dot(normal, ovmin) > 0.0f)
	{
		return false;
	}

	if (Vector3::Dot(normal, ovmax) >= 0.0f)
	{
		return true;
	}

	return false;
}

bool Intersection::AABBSphere(const AABB & aabb, const Sphere & s)
{
	float sqDist = aabb.DistanceSqrTo(s.GetCenter());
	return sqDist <= s.GetRadius() * s.GetRadius();
}

bool Intersection::AABBSphere(const AABB & aabb, const Sphere & s, Vector3 & point, Vector3 & normal)
{
	const Vector3 &center = s.GetCenter();
	const float radius = s.GetRadius();

	// closest point on aabb to sphere's center
	point = aabb.GetClosestPoint(center);

	// distance from closest point on aabb to center
	Vector3 v = center - point;
	float d = v.LengthSqr();

	if (d <= radius * radius)
	{
		// normalize
		normal = v / Sqrt(d);
		return true;
	}

	return false;
}

bool Intersection::AABBAABB(const AABB & aabb1, const AABB & aabb2)
{
	const Vector3 &mina = aabb1.GetMin();
	const Vector3 &maxa = aabb1.GetMax();
	const Vector3 &minb = aabb2.GetMin();
	const Vector3 &maxb = aabb2.GetMax();

	for (int i = 0; i < 3; i++)
	{
		if (maxa[i] < minb[i] || mina[i] > maxb[i])
		{
			return false;
		}
	}

	return true;
}

bool Intersection::AABBAABB(const AABB & aabb1, const AABB & aabb2, Vector3 & point, Vector3 & normal)
{
	const Vector3 &mina = aabb1.GetMin();
	const Vector3 &maxa = aabb1.GetMax();
	const Vector3 &minb = aabb2.GetMin();
	const Vector3 &maxb = aabb2.GetMax();

	for (int i = 0; i < 3; i++)
	{
		if (maxa[i] < minb[i] || mina[i] > maxb[i])
		{
			return false;
		}
	}

	Vector3 e = aabb2.GetCenter() - aabb1.GetCenter();
	normal = e.GetNormalized();
	point = aabb1.GetCenter() + e * 0.5f;

	return true;
}

bool Intersection::AABBPlane(const AABB & aabb, const Plane & p)
{
	const Vector3 c = aabb.GetCenter();
	const Vector3 e = aabb.GetMax() - c; // positive extents
	const Vector3 &n = p.GetNormal();

	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = e[0] * Abs(n[0]) + e[1] * Abs(n[1]) + e[2] * Abs(n[2]);

	// Compute distance of box center from plane
	float s = p.PlaneDot(c);

	// Intersection occurs when distance s falls within [-r,+r] interval
	// return Abs(s) <= r;

	// Intersection with negative halfspace occurs 
	// when distance s falls within (-inf,+r] interval
	return s <= r;
}

bool Intersection::RaySphere(const Ray & ray, const Sphere & s, Vector3 & point)
{
	Vector3 m = ray.GetStart() - s.GetCenter();
	float b = Vector3::Dot(m, ray.GetDirection());
	float c = m.LengthSqr() - s.GetRadius() * s.GetRadius();

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	
	// A negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f)
	{
		return false;
	}
	
	// Ray now found to intersect sphere, compute smallest t value of intersection
	float t = -b - Sqrt(discr);
	
	// If t is negative, ray started inside sphere so clamp t to zero
	if (t < 0.0f)
	{
		t = 0.0f;
	}

	point = ray.GetStart() + ray.GetDirection() * t;
	
	return true;
}

bool Intersection::RaySphere(const Ray & ray, const Sphere & s, Vector3 & point, Vector3 & normal)
{
	if (!RaySphere(ray, s, point))
	{
		return false;
	}

	normal = (point - s.GetCenter()).GetNormalized();

	return true;
}

bool Intersection::RayAABB(const Ray & ray, const AABB & aabb, Vector3 & point, float & t)
{
	const Vector3 &p = ray.GetStart();
	const Vector3 &d = ray.GetDirection();
	const Vector3 &min = aabb.GetMin();
	const Vector3 &max = aabb.GetMax();

	const float eps = 0.0001f;

	float tmin = 0.0f;
	float tmax = FLT_MAX;

	// For all three sections
	for (int i = 0; i < 3; i++) 
	{
		if (Abs(d[i]) < eps)
		{
			// Ray is parallel to section. No hit if origin not within section
			if (p[i] < min[i] || p[i] > max[i])
			{
				return false;
			}
		}
		else
		{
			// Compute intersection t value of ray with near and far plane of section
			float ood = 1.0f / d[i];
			float t1 = (min[i] - p[i]) * ood;
			float t2 = (max[i] - p[i]) * ood;
			
			// Make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			// Compute the intersection of section intersection intervals
			if (t1 > tmin)
			{
				tmin = t1;
			}
			
			if (t2 < tmax)
			{
				tmax = t2;
			}
			
			// Exit with no collision as soon as section intersection becomes empty
			if (tmin > tmax)
			{
				return false;
			}
		}
	}

	// Ray intersects all 3 sections
	point = p + d * tmin;
	t = tmin;

	return true;
}

bool Intersection::RayPlane(const Ray & ray, const Plane & p, Vector3 & point)
{
	const float eps = 0.0001f;
	float denom = Vector3::Dot(ray.GetDirection(), p.GetNormal());

	// Are parallel?
	if (Abs(denom) > eps)
	{
		float t = -p.PlaneDot(ray.GetStart()) / denom;

		if (t >= 0.0f)
		{
			point = ray.GetStart() + ray.GetDirection() * t;
			return true;
		}
	}

	return false;
}

bool Intersection::RayTriangle(const Ray & ray, const Triangle & t, Vector3 & barycentric)
{
	return SegRayTriangle(ray.GetStart(), ray.GetStart() + ray.GetDirection(), t, barycentric, false);
}

bool Intersection::SegmentAABB(const Vector3 & p0, const Vector3 & p1, const AABB & aabb, Vector3 & point, float & t)
{
	const float eps = 0.0001f;

	Vector3 c = aabb.GetCenter();
	Vector3 e = aabb.GetMax() - c;	// Box halflength extents
	Vector3 m = (p0 + p1) * 0.5f;	// Segment midpoint
	Vector3 d = p1 - m;				// Segment halflength vector
	
	m = m - c; // Translate box and segment to origin
			   // Try world coordinate axes as separating axes
	
	float adx = Abs(d[0]);
	if (Abs(m[0]) > e[0] + adx) 
	{
		return false; 
	}

	float ady = Abs(d[1]);
	if (Abs(m[1]) > e[1] + ady)
	{
		return false;
	}
	
	float adz = Abs(d[2]);
	if (Abs(m[2]) > e[2] + adz)
	{
		return false;
	}

	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis (see text for detail)
	adx += eps; ady += eps; adz += eps;

	// Try cross products of segment direction vector with coordinate axes
	if (Abs(m[1] * d[2] - m[2] * d[1]) > e[1] * adz + e[2] * ady) { return false; }
	if (Abs(m[2] * d[0] - m[0] * d[2]) > e[0] * adz + e[2] * adx) { return false; }
	if (Abs(m[0] * d[1] - m[1] * d[0]) > e[0] * ady + e[1] * adx) { return false; }
	
	// No separating axis found; segment must be overlapping AABB
	return true;
}

bool Intersection::SegmentPlane(const Vector3 & p0, const Vector3 & p1, const Plane & p, Vector3 & point)
{
	const float eps = 0.0001f;

	Vector3 dir = p1 - p0;
	float denom = Vector3::Dot(dir, p.GetNormal());

	// Are parallel?
	if (Abs(denom) > eps)
	{
		float t = -p.PlaneDot(p0) / denom;

		if (t >= 0.0f && t <= 1.0f)
		{
			point = p0 + dir * t;
			return true;
		}
	}

	return false;
}

bool Intersection::SegmentTriangle(const Vector3 & p, const Vector3 & q, const Triangle & tr, Vector3 & barycentric)
{
	return SegRayTriangle(p, q, tr, barycentric, true);
}

bool Intersection::SphereInsideFrustum(const Frustum & f, const Sphere & s)
{
	for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
	{
		// if sphere doesn't intersect with negative halfspace
		// i.e. sphere is not inside
		if (!SpherePlane(s, f.GetPlane(i)))
		{
			return false;
		}
	}

	return true;
}

bool Intersection::AABBInsideFrustum(const Frustum & f, const AABB & aabb)
{
	for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
	{
		// if box doesn't intersect with negative halfspace
		// i.e. box is not inside
		if (!AABBPlane(aabb, f.GetPlane(i)))
		{
			return false;
		}
	}

	return true;
}

bool Intersection::SegRayTriangle(const Vector3 & p, const Vector3 & q, const Triangle & tr, Vector3 & barycentric, bool isSegment)
{
	float u, v, w; // Barycentric coords of intersection
	float t;

	Vector3 ab = tr.B - tr.A;
	Vector3 ac = tr.C - tr.A;
	Vector3 qp = p - q;

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	Vector3 n = Vector3::Cross(ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = Vector3::Dot(qp, n);
	if (d <= 0.0f) { return false; }

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	Vector3 ap = p - tr.A;
	t = Vector3::Dot(ap, n);
	if (t < 0.0f) { return false; }

	if (isSegment)
	{
		// For segment; exclude this code line for a ray test
		if (t > d) { return false; }
	}

	// Compute barycentric coordinate components and test if within bounds
	Vector3 e = Vector3::Cross(qp, ap);

	v = Vector3::Dot(ac, e);
	if (v < 0.0f || v > d) { return false; }

	w = -Vector3::Dot(ab, e);
	if (w < 0.0f || v + w > d) { return false; }

	// Segment/ray intersects triangle. Perform delayed division and
	// compute the last barycentric coordinate component
	float ood = 1.0f / d;
	t *= ood;
	v *= ood;
	w *= ood;

	u = 1.0f - v - w;

	barycentric[0] = u;
	barycentric[1] = v;
	barycentric[2] = w;
	return true;
}

bool Intersection::SphereSphere(const Sphere & s1, const Sphere & s2, Vector3 & point, Vector3 & normal, float & penetration)
{
	Vector3 delta = s2.GetCenter() - s1.GetCenter();

	float d = delta.LengthSqr();
	float radiusSum = s1.GetRadius() + s2.GetRadius();

	// if intersecting, calculate normal, position, penetration
	if (d <= radiusSum * radiusSum)
	{
		d = Sqrt(d);

		// normalize
		normal = delta / d;
		point = s1.GetCenter() + normal * s1.GetRadius();

		penetration = radiusSum - d;

		return true;
	}

	return false;
}

bool Intersection::SpherePlane(const Sphere & s, const Plane & p, float & penetration)
{
	float distToCenter = p.PlaneDot(s.GetCenter());
	float radius = s.GetRadius();
	
	if (distToCenter <= radius)
	{
		penetration = radius - distToCenter;
		return true;
	}

	return false;
}

bool Intersection::AABBSphere(const AABB & aabb, const Sphere & s, Vector3 & point, Vector3 & normal, float & penetration)
{
	const Vector3 &center = s.GetCenter();
	const float radius = s.GetRadius();

	// closest point on aabb to sphere's center
	point = aabb.GetClosestPoint(center);

	// distance from closest point on aabb to center
	Vector3 v = center - point;
	float d = v.LengthSqr();

	if (d <= radius * radius)
	{
		d = Sqrt(d);

		// normalize
		normal = v / d;
		penetration = radius - d;
		return true;
	}

	return false;
}

bool Intersection::AABBAABB(const AABB & aabb1, const AABB & aabb2, Vector3 & point, Vector3 & normal, float & penetration)
{
	penetration = FLT_MAX;

	const Vector3 &mina = aabb1.GetMin();
	const Vector3 &maxa = aabb1.GetMax();
	const Vector3 &minb = aabb2.GetMin();
	const Vector3 &maxb = aabb2.GetMax();

	Vector3 unit[] = { Vector3(1.0f,0.0f,0.0f),Vector3(0.0f,1.0f,0.0f) ,Vector3(0.0f,0.0f,1.0f) };

	for (int i = 0; i < 3; i++)
	{
		float d0 = maxb[i] - mina[i];
		float d1 = maxa[i] - minb[i];

		if (d0 <= 0.0f || d1 <= 0.0f)
		{
			return false;
		}

		float overlap = d0 < d1 ? d0 : -d1;

		Vector3 sep = unit[i] * overlap;

		if (overlap < penetration)
		{
			penetration = overlap;
			normal = overlap > 0.0f ? unit[i] : -unit[i];
		}
	}

	return true;
}

bool Intersection::AABBPlane(const AABB & aabb, const Plane & p, float & penetration)
{
	const Vector3 c = aabb.GetCenter();
	const Vector3 e = aabb.GetMax() - c; // positive extents
	const Vector3 &n = p.GetNormal();

	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	float r = e[0] * Abs(n[0]) + e[1] * Abs(n[1]) + e[2] * Abs(n[2]);

	// Compute distance of box center from plane
	penetration = p.PlaneDot(c);

	// Intersection with negative halfspace occurs 
	// when distance s falls within (-inf,+r] interval
	return penetration <= r;
}

bool Intersection::MeshSphere(const StaticArray<Triangle>& triangles, const Sphere & s, Vector3 & point, Vector3 & normal, float & penetration)
{
	UINT size = triangles.GetSize();

	Vector3 center = s.GetCenter();
	float radius = s.GetRadius();
	float radiusSqr = radius * radius;

	for (UINT i = 0; i < size; i++)
	{
		// closest point
		point = triangles[i].GetClosestPoint(center);

		// from closest point on triangle to center
		Vector3 v = point - center;
		float lengthSqr = v.LengthSqr();

		if (lengthSqr <= radiusSqr)
		{
			normal = triangles[i].GetNormal();
			penetration = radius - v.Length();
			return true;
		}
	}

	return false;
}

bool Intersection::MeshAABB(const StaticArray<Triangle>& triangles, const AABB & aabb, Vector3 & point, Vector3 & normal, float & penetration)
{
	UINT size = triangles.GetSize();

	for (UINT i = 0; i < size; i++)
	{
		const Triangle &t = triangles[i];

		if (TriangleAABB(t, aabb))
		{
			normal = t.GetNormal();
			point = t.GetClosestPoint(aabb.GetCenter());

			Plane p = Plane(normal, point);
			AABBPlane(aabb, p, penetration);

			return true;
		}
	}

	return false;
}
