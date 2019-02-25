#pragma once
#include "Vector.h"

// Represents ray: start + t * direction, where t is in range [0, +inf)
class Ray
{
private:
	Vector3 start;
	Vector3 direction;	// direction vector, normalized

public:
	inline Ray(const Vector3 &start, const Vector3 &direction);
	inline Ray(const Ray &r);

	inline Ray operator=(const Ray &r);
	inline bool operator==(const Ray &r) const;
	inline bool operator!=(const Ray &r) const;

	inline const Vector3 &GetStart() const;
	inline const Vector3 &GetDirection() const;
};

inline Ray::Ray(const Vector3 &start, const Vector3 &direction)
{
	this->start = start;
	this->direction = direction.GetNormalized();
}

inline Ray::Ray(const Ray &r)
{
	start = r.start;
	
	// direction of ray "r" is normalized
	direction = r.direction;
}

inline Ray Ray::operator=(const Ray &r)
{
	return Ray(r.start, r.direction);
}

inline bool Ray::operator==(const Ray &r) const
{
	return start == r.start && direction == r.direction;
}

inline bool Ray::operator!=(const Ray &r) const
{
	return start != r.start || direction != r.direction;
}

inline const Vector3 & Ray::GetStart() const
{
	return start;
}

inline const Vector3 & Ray::GetDirection() const
{
	return direction;
}
