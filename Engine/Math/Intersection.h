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
	static bool SegmentPlane(const Vector3 &p0, const Vector3 &p1, const Plane &p, Vector3 &point);
	// Test segment and AABB intersection
	//   "barycentric" is a barycentric coords on triangle of intersection
	static bool SegmentTriangle(const Vector3 &p0, const Vector3 &p1, const Triangle &t, Vector3 &barycentric);

	static bool SphereInsideFrustum(const Frustum &f, const Sphere &s);
	static bool AABBInsideFrustum(const Frustum &f, const AABB &aabb);

	// Test triangle and sphere intersection
	//   "point" is a closest point on triangle from sphere center
	static bool TriangleSphere(const Triangle &t, const Sphere &s, Vector3 &point);
	static bool TriangleAABB(const Triangle &t, const AABB &aabb);
	// Test triangle and plane intersection
	// If intersect then result is a segment (p0, p1)
	static bool TrianglePlane(const Triangle &t, const Plane &plane, Vector3 &start, Vector3 &end);

	static bool MeshSphere(const MeshResource &mesh, const Sphere &s, Vector3 &point);
	static bool MeshAABB(const MeshResource &mesh, const AABB &aabb);
	static bool MeshPlane(const MeshResource &mesh, const Plane &p, Vector3 &start, Vector3 &end);
	// Test mesh and ray.
	//   "worldPoint" is a intersection point in world space
	//   "normal" is an intersection normal
	static bool MeshRay(const MeshResource &mesh, const Ray &ray, Vector3 &worldPoint, Vector3 & normal);
	// Test mesh and segment.
	//   "worldPoint" is a intersection point in world space
	static bool MeshSegment(const MeshResource &mesh, const Vector3 &start, const Vector3 &end, Vector3 &worldPoint);

private:
	static bool SegRayTriangle(const Vector3 &p, const Vector3 &q, const Triangle &tr, Vector3 &barycentric, bool isSegment);
	static bool PlaneBoxOverlap(const Vector3 &normal, const Vector3 &vert, const Vector3 &maxbox);
};