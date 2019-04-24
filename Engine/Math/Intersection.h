#pragma once

class Intersection
{
public:
	static bool SphereSphere(const Sphere &s1, const Sphere &s2);
	static bool SphereSphere(const Sphere &s1, const Sphere &s2, Vector3 &point, Vector3 &normal);
	// Does sphere intersects with negative halfspace of plane?
	// Note: consider that negative halfspace is solid
	static bool SpherePlane(const Sphere &s, const Plane &p);
	static bool SphereAABB(const Sphere &s, const AABB &aabb);

	
	static bool AABBSphere(const AABB &aabb, const Sphere &s);
	static bool AABBSphere(const AABB &aabb, const Sphere &s, Vector3 &point, Vector3 &normal);
	static bool AABBAABB(const AABB &aabb1, const AABB &aabb2);
	static bool AABBAABB(const AABB &aabb1, const AABB &aabb2, Vector3 &point, Vector3 &normal);
	// Does AABB intersects with negative halfspace of plane?
	// Note: consider that negative halfspace is solid
	static bool AABBPlane(const AABB &aabb, const Plane &p);

	// Test ray and sphere intersection
	//   "point" is a intersection point on sphere
	static bool RaySphere(const Ray &ray, const Sphere &s, Vector3 &point, float &t);
	// Test ray and sphere intersection
	//   "point" is a intersection point on sphere
	//   "normal" is a normal on sphere in intersection point
	static bool RaySphere(const Ray &ray, const Sphere &s, Vector3 &point, Vector3 &normal, float &t);
	// Test ray and AABB intersection
	//   "point" is a intersection point on AABB
	//   "t" is a t of ray
	static bool RayAABB(const Ray &ray, const AABB &aabb, Vector3 &point, float &t);
	static bool RayPlane(const Ray &ray, const Plane &p, Vector3 &point);
	static bool RayTriangle(const Ray &ray, const Triangle &tr, Vector3 &barycentric, float &t);


	static bool SegmentAABB(const Vector3 &p0, const Vector3 &p1, const AABB &aabb, Vector3 &point, float &t);
	static bool SegmentPlane(const Vector3 &p0, const Vector3 &p1, const Plane &p, Vector3 &point);
	// Test segment and AABB intersection
	//   "barycentric" is a barycentric coords on triangle of intersection
	static bool SegmentTriangle(const Vector3 &p0, const Vector3 &p1, const Triangle &tr, Vector3 &barycentric, float &t);


	static bool SphereInsideFrustum(const Frustum &f, const Sphere &s);
	static bool AABBInsideFrustum(const Frustum &f, const AABB &aabb);


	// Test triangle and sphere intersection
	//   "point" is a closest point on triangle from sphere center
	static bool TriangleSphere(const Triangle &t, const Sphere &s, Vector3 &point);
	static bool TriangleAABB(const Triangle &t, const AABB &aabb);
	static bool TriangleAABB(const Triangle &t, const AABB &aabb, Vector3 &point, Vector3 &normal);
	// Test triangle and plane intersection
	// If intersect then result is a segment (p0, p1)
	static bool TrianglePlane(const Triangle &t, const Plane &plane, Vector3 &start, Vector3 &end);


	static bool MeshSphere(const StaticArray<Triangle> &triangles, const Sphere &s, Vector3 &point, Vector3 &normal);
	static bool MeshAABB(const StaticArray<Triangle> &triangles, const AABB &aabb);
	static bool MeshAABB(const StaticArray<Triangle> &triangles, const AABB &aabb, Vector3 &point, Vector3 &normal);
	static bool MeshPlane(const StaticArray<Triangle> &triangles, const Plane &p, Vector3 &start, Vector3 &end);
	// Test mesh and ray.
	//   "worldPoint" is a intersection point in world space
	//   "normal" is an intersection normal
	static bool MeshRay(const StaticArray<Triangle> &triangles, const Ray &ray, Vector3 &worldPoint, Vector3 & normal, float &t);
	// Test mesh and segment.
	//   "worldPoint" is a intersection point in world space
	static bool MeshSegment(const StaticArray<Triangle> &triangles, const Vector3 &start, const Vector3 &end, Vector3 &worldPoint, float &t);


	// With penetration data
	static bool SphereSphere(const Sphere &s1, const Sphere &s2, Vector3 &point, Vector3 &normal, float &penetration);
	static bool SpherePlane(const Sphere &s, const Plane &p, float &penetration);
	static bool SphereAABB(const Sphere &s, const AABB &aabb, Vector3 &point, Vector3 &normal, float &penetration);
	static bool AABBSphere(const AABB &aabb, const Sphere &s, Vector3 &point, Vector3 &normal, float &penetration);
	static bool AABBAABB(const AABB &aabb1, const AABB &aabb2, Vector3 &point, Vector3 &normal, float &penetration);
	static bool AABBPlane(const AABB &aabb, const Plane &p, float &penetration);
	static bool MeshSphere(const StaticArray<Triangle> &triangles, const Sphere &s, Vector3 &point, Vector3 &normal, float &penetration);
	static bool MeshAABB(const StaticArray<Triangle> &triangles, const AABB &aabb, Vector3 &point, Vector3 &normal, float &penetration);
	
	static bool MeshSphere(const StaticArray<Triangle> &triangles, const DynamicArray<int>& indices, const Sphere &s, Vector3 &point, Vector3 &normal, float &penetration);
	static bool MeshAABB(const StaticArray<Triangle>& triangles, const DynamicArray<int>& indices, const AABB & aabb, Vector3 & point, Vector3 & normal, float & penetration);

private:
	static bool SegRayTriangle(const Vector3 &p, const Vector3 &q, const Triangle &tr, Vector3 &barycentric, bool isSegment, float &t);
	static bool PlaneBoxOverlap(const Vector3 &normal, const Vector3 &vert, const Vector3 &maxbox);
};