#include "Intersection.h"
#include "AABB.h"
#include "Frustum.h"
#include "Plane.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"

#include <limits>

bool Intersection::SphereSphere(const Sphere & s1, const Sphere & s2)
{
	float d = Vector3::DistanceSqr(s1.GetCenter(), s2.GetCenter());
	float radiusSum = s1.GetRadius() + s2.GetRadius();
	
	return d <= radiusSum *radiusSum;
}

bool Intersection::SpherePlane(const Sphere & s, const Plane & p)
{
	float distToCenter = p.PlaneDot(s.GetCenter());
	return distToCenter <= s.GetRadius();
}

bool Intersection::SphereTriangle(const Sphere & s, const Vector3 & a, const Vector3 & b, const Vector3 & c, Vector3 & point)
{
	// closest point
	point = Triangle::GetClosestPoint(s.GetCenter(), a, b, c);

	// distance from closest point on triangle to center
	Vector3 v = point - s.GetCenter();
	return v.LengthSqr() <= s.GetRadius() * s.GetRadius();
}

bool Intersection::AABBSphere(const AABB & aabb, const Sphere & s)
{
	float sqDist = aabb.DistanceSqrTo(s.GetCenter());
	return sqDist <= s.GetRadius() * s.GetRadius();
}

bool Intersection::AABBSphere(const AABB & aabb, const Sphere & s, Vector3 & point)
{
	// closest point on aabb to sphere's center
	point = aabb.GetClosestPoint(s.GetCenter());

	// distance from closest point on aabb to center
	Vector3 v = point - s.GetCenter();
	return v.LengthSqr() <= s.GetRadius() * s.GetRadius();
}

bool Intersection::AABBAABB(const AABB & aabb1, const AABB & aabb2)
{
	const Vector3 &mina = aabb1.GetMin();
	const Vector3 &minb = aabb2.GetMax();
	const Vector3 &maxa = aabb1.GetMin();
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

	if (Abs(denom) > eps)
	{
		float t = -p.PlaneDot(ray.GetStart()) / denom;

		if (t >= 0)
		{
			point = ray.GetStart() + ray.GetDirection() * t;
			return true;
		}
	}

	return false;
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

bool Intersection::SegmentTriangle(const Vector3 & p, const Vector3 & q, const Triangle & tr, Vector3 & barycentric)
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
	if (t > d) { return false; }	// For segment; exclude this code line for a ray test
									
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

bool Intersection::SphereInsideFrustum(const Frustum & f, const Sphere & s)
{
	const Vector3 &center = s.GetCenter();
	const float radius = s.GetRadius();

	for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
	{
		const Plane plane = f.GetPlane(i);
		if (!plane.IsInFront(center, radius))
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
		const Plane plane = f.GetPlane(i);
		if (!plane.IsInside(aabb))
		{
			return false;
		}
	}

	return true;
}
