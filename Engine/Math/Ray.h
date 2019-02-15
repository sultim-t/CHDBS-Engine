#pragma once

#include "Vector.h"

struct Ray
{
private:
	Vector3 start;
	Vector3 end;

public:
	inline Ray(const Vector3 &start, const Vector3 &end);
	inline Ray(const Ray &r);

	inline Ray operator=(const Ray &r);
	inline bool operator==(const Ray &r);
	inline bool operator==(const Ray &r) const;
	inline bool operator!=(const Ray &r);
};

inline Ray::Ray(const Vector3 & start, const Vector3 & end)
{
	this->start = start;
	this->end = end;
}

inline Ray::Ray(const Ray & r)
{
	start = r.start;
	end = r.end;
}

inline Ray Ray::operator=(const Ray & r)
{
	return Ray(r.start, r.end);
}

inline bool Ray::operator==(const Ray & r)
{
	return start == r.start && end == r.end;
}

inline bool Ray::operator==(const Ray & r) const
{
	return start == r.start && end == r.end;
}

inline bool Ray::operator!=(const Ray & r)
{
	return start != r.start || end != r.end;
}
