#pragma once

#include "Vector.h"

struct Ray
{
private:
	Vector3 start;
	Vector3 direction;

public:
	inline Ray(const Vector3 &start, const Vector3 &direction);
	inline Ray(const Ray &r);

	inline Ray operator=(const Ray &r);
	inline bool operator==(const Ray &r);
	inline bool operator==(const Ray &r) const;
	inline bool operator!=(const Ray &r);
};

inline Ray::Ray(const Vector3 &start, const Vector3 &direction)
{
	this->start = start;
	this->direction = direction;
}

inline Ray::Ray(const Ray &r)
{
	start = r.start;
	direction = r.direction;
}

inline Ray Ray::operator=(const Ray &r)
{
	return Ray(r.start, r.direction);
}

inline bool Ray::operator==(const Ray &r)
{
	return start == r.start && direction == r.direction;
}

inline bool Ray::operator==(const Ray &r) const
{
	return start == r.start && direction == r.direction;
}

inline bool Ray::operator!=(const Ray &r)
{
	return start != r.start || direction != r.direction;
}
