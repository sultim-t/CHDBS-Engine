#pragma once

class Intersection
{
public:
	static bool SphereSphere(const Sphere &s1, const Sphere &s2);
	// Does sphere intersects with negative halfspace of plane?
	// Note: consider that negative halfspace is solid
	static bool SpherePlane(const Sphere &s, const Plane &p);

	static bool AABBSphere(const AABB &aabb, const Sphere &s);
	static bool AABBSphere(const AABB &aabb, const Sphere &s, Vector3 &point);
	static bool AABBAABB(const AABB &aabb1, const AABB &aabb2);
	// Does AABB intersects with negative halfspace of plane?
	// Note: consider that negative halfspace is solid
	static bool AABBPlane(const AABB &aabb, const Plane &p);

	// Test ray and sphere intersection
	//   "point" is a intersection point on sphere
	static bool RaySphere(const Ray &ray, const Sphere &s, Vector3 &point);
	// Test ray and AABB intersection
	//   "point" is a intersection point on AABB
	//   "t" is a t of ray
	static bool RayAABB(const Ray &ray, const AABB &aabb, Vector3 &point, float &t);
	static bool RayPlane(const Ray &ray, const Plane &p, Vector3 &point);
	static bool RayTriangle(const Ray &ray, const Triangle &t, Vector3 &barycentric);

	static bool SegmentAABB(const Vector3 &p0, const Vector3 &p1, const AABB &aabb, Vector3 &point, float &t);
	// Test segment and AABB intersection
	//   "barycentric" is a barycentric coords on triangle of intersection
	static bool SegmentTriangle(const Vector3 &p0, const Vector3 &p1, const Triangle &t, Vector3 &barycentric);

	static bool SphereInsideFrustum(const Frustum &f, const Sphere &s);
	static bool AABBInsideFrustum(const Frustum &f, const AABB &aabb);

	// Test triangle and sphere intersection
	//   "point" is a closest point on triangle from sphere center
	static bool TriangleSphere(const Triangle &t, const Sphere &s, Vector3 &point);
	static bool TriangleAABB(const Triangle &t, const AABB &aabb);
	static bool TrianglePlane(const Triangle &t, const Plane &p);

	static bool MeshSphere(const Mesh &f, const Sphere &s);
	static bool MeshAABB(const Mesh &f, const AABB &aabb);
	static bool MeshPlane(const Mesh &f, const Plane &p);

private:
	static bool SegRayTriangle(const Vector3 &p, const Vector3 &q, const Triangle &tr, Vector3 &barycentric, bool isSegment);
	static bool PlaneBoxOverlap(const Vector3 &normal, const Vector3 &vert, const Vector3 &maxbox);
};