#pragma once

#include "Vector.h"

struct Quaternion
{
private:
	union
	{
		float quat[4];
		struct
		{
			float w;
			float x;
			float y;
			float z;
		};
	};
public:
	inline Quaternion();
	inline Quaternion(float a);
	inline Quaternion(float w, float x, float y, float z);
	inline Quaternion(const Vector3 &euler);
	inline Quaternion(const Quaternion &q);

	inline const float operator[](int index) const;
	inline float& operator[](int index);

	inline const Quaternion operator+(const Quaternion &r) const;
	inline Quaternion &operator+=(const Quaternion &r);
	inline const Quaternion operator-(const Quaternion &r) const;
	inline Quaternion &operator-=(const Quaternion &r);

	inline const Quaternion operator*(const float scale) const;
	inline Quaternion &operator*=(const float scale);
	inline const Quaternion operator/(const float scale) const;
	inline Quaternion &operator/=(const float scale);

	inline const Quaternion operator*(const Quaternion &r) const;
	inline Quaternion &operator*=(const Quaternion &r);

	inline const Vector3 operator*(const Vector3 &v) const;

	static Quaternion Identity();

	static float Dot(Quaternion &a, Quaternion &b);
	inline void Normalize();

	// Counvert from euler in degrees to quat
	static void FromEuler(const Vector3 &euler, Quaternion &quat);
	// Convert euler in degrees to current quaternion
	inline void FromEuler(const Vector3 &euler);

	static void ToEuler(const Quaternion &quat, Vector3 &euler);
	inline const void ToEuler(Vector3 &euler) const;
	static Vector3 ToEuler(const Quaternion &quat);
	inline const Vector3 ToEuler() const;

	// Convert from axis and angle
	inline void FromAxisAngle(const Vector3 &axis, float angle);
	// Convert to axis and angle in Radians
	inline void ToAxisAngle(Vector3 &axis, float &angle) const;

	// Interpolates between a and b, normalizes the result, t is clamped to [0,1]
	static Quaternion Lerp(Quaternion &a, Quaternion &b, float t);
	// Spherically interpolates between a and b, t is clamped to [0,1]
	static Quaternion Slerp(Quaternion &a, Quaternion &b, float t);

	inline Vector3 RotateVector(Vector3 v) const;

	inline Vector3 GetForward() const;
	inline Vector3 GetRight() const;
	inline Vector3 GetUp() const;
};

inline Quaternion::Quaternion()
{
}

inline Quaternion::Quaternion(float a)
{
	w = x = y = z = a;
}

inline Quaternion::Quaternion(float w, float x, float y, float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

inline Quaternion::Quaternion(const Vector3& euler)
{
	FromEuler(euler, *this);
}

inline Quaternion::Quaternion(const Quaternion & q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}

inline const float Quaternion::operator[](int index) const
{
	ASSERT(index >= 0 || index < 4);
	return quat[index];
}

inline float & Quaternion::operator[](int index)
{
	ASSERT(index >= 0 || index < 4);
	return quat[index];
}

inline const Quaternion Quaternion::operator+(const Quaternion & r) const
{
	return Quaternion(w + r.w, x + r.x, y + r.y, z + r.z);
}

inline Quaternion & Quaternion::operator+=(const Quaternion & r)
{
	w += r.w;
	x += r.x;
	y += r.y;
	z += r.z;

	return *this;
}

inline const Quaternion Quaternion::operator-(const Quaternion & r) const
{
	return Quaternion(w - r.w, x - r.x, y - r.y, z - r.z);
}

inline Quaternion & Quaternion::operator-=(const Quaternion & r)
{
	w -= r.w;
	x -= r.x;
	y -= r.y;
	z -= r.z;

	return *this;
}

inline const Quaternion Quaternion::operator*(const Quaternion & r) const
{
	return Quaternion(
		w * r.w - x * r.x - y * r.y - z * r.z,
		w * r.x + x * r.w + y * r.z - z * r.y,
		w * r.y - x * r.z + y * r.w + z * r.x,
		w * r.z + x * r.y - y * r.x + z * r.w);
}

inline Quaternion & Quaternion::operator*=(const Quaternion & r)
{
	*this = (*this) * r;
	return *this;
}

inline const Vector3 Quaternion::operator*(const Vector3 & v) const
{
	return RotateVector(v);
}

inline Quaternion Quaternion::Identity()
{
	return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

inline const Quaternion Quaternion::operator*(const float scale) const
{
	return Quaternion(
	  w * scale,
	  x * scale,
	  y * scale,
	  z * scale);
}

inline Quaternion & Quaternion::operator*=(const float scale)
{
	w *= scale;
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}

inline const Quaternion Quaternion::operator/(const float scale) const
{
	float inv = 1.0f / scale;

	return Quaternion(
		w * inv,
		x * inv,
		y * inv,
		z * inv);
}

inline Quaternion & Quaternion::operator/=(const float scale)
{
	float inv = 1.0f / scale;

	w *= inv;
	x *= inv;
	y *= inv;
	z *= inv;

	return *this;
}

inline void Quaternion::ToEuler(const Quaternion & q, Vector3& euler)
{
	// roll
	float sinr = +2.0f * (q.w * q.x + q.y * q.z);
	float cosr = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	euler[0] = RAD2DEG(ATan2(sinr, cosr));

	// pitch
	float sinp = +2.0f * (q.w * q.y - q.z * q.x);
	// clamping
	if (Abs(sinp) >= 1.0f)
	{
		euler[1] = RAD2DEG(CopySign(PI / 2, sinp)); // clamp to 90 degrees
	}
	else
	{
		euler[1] = RAD2DEG(ASin(sinp));
	}

	// yaw
	float siny = +2.0f * (q.w * q.z + q.x * q.y);
	float cosy = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	euler[2] = RAD2DEG(ATan2(siny, cosy));
}

const inline void Quaternion::ToEuler(Vector3 & euler) const
{
	ToEuler(*this, euler);
}

inline Vector3 Quaternion::ToEuler(const Quaternion & quat)
{
	Vector3 result;
	ToEuler(quat, result);

	return result;
}

const inline Vector3 Quaternion::ToEuler() const
{
	return ToEuler(*this);
}

inline void Quaternion::FromAxisAngle(const Vector3 & axis, float angle)
{
	w = Cos(angle * 0.5f);

	float s = Sin(angle * 0.5f);
	x = axis[0] * s;
	y = axis[1] * s;
	z = axis[2] * s;
}

inline void Quaternion::ToAxisAngle(Vector3 & axis, float & angle) const
{
	angle = 2 * ACos(w);

	float s = Sqrt(1 - w * w);

	if (ABS(s) >= 0.001f)
	{
		axis[0] = x / s;
		axis[1] = y / s;
		axis[2] = z / s;
	}
	else
	{
		// angle is zero
		axis[0] = 1;
		axis[1] = 0;
		axis[2] = 0;
	}
}

inline float Quaternion::Dot(Quaternion & a, Quaternion & b)
{
	return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

inline void Quaternion::Normalize()
{
	Vector4 q = (w, x, y, z);
	q.Normalize();

	w = q[0];
	x = q[1];
	y = q[2];
	z = q[3];
}

inline void Quaternion::FromEuler(const Vector3& euler, Quaternion & q)
{
	float pitch = DEG2RAD(euler[PITCH]);
	float roll = DEG2RAD(euler[ROLL]);
	float yaw = DEG2RAD(euler[YAW]);

	float cy = Cos(yaw * 0.5f);
	float sy = Sin(yaw * 0.5f);

	float cr = Cos(roll * 0.5f);
	float sr = Sin(roll * 0.5f);

	float cp = Cos(pitch * 0.5f);
	float sp = Sin(pitch * 0.5f);

	q.w = cy * cr * cp + sy * sr * sp;
	q.x = cy * sr * cp - sy * cr * sp;
	q.y = cy * cr * sp + sy * sr * cp;
	q.z = sy * cr * cp - cy * sr * sp;
}

inline void Quaternion::FromEuler(const Vector3& euler)
{
	FromEuler(euler, *this);
}

inline Quaternion Quaternion::Lerp(Quaternion & a, Quaternion & b, float t)
{
	Quaternion q;

	float it = 1 - t;

	q.x = it * a.x + t * b.x;
	q.y = it * a.y + t * b.y;
	q.z = it * a.z + t *b.z;
	q.w = it * a.w + t * b.w;

	q.Normalize();

	return q;
}

inline Quaternion Quaternion::Slerp(Quaternion & a, Quaternion & b, float t)
{
	Quaternion q;

	float it = 1 - t;

	float theta = ACos(a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w);
	float s = Sin(theta);

	float la = Sin(it * theta) / s;
	float lb = Sin(t * theta) / s;

	q.x = la * a.x + lb * b.x;
	q.y = la * a.y + lb * b.y;
	q.z = la * a.z + lb * b.z;
	q.w = la * a.w + lb * b.w;

	q.Normalize();

	return q;
}

inline Vector3 Quaternion::RotateVector(Vector3 v) const
{
	Vector3 result;

	Vector3 q = Vector3(quat[0], quat[1], quat[2]);
	Vector3 cross = Vector3::Cross(q, v) * 2.0f;
	
	result = v + (cross * quat[3]) + Vector3::Cross(q, cross);
	return result;
}

inline Vector3 Quaternion::GetForward() const
{
	return RotateVector(Vector3(1.0f, 0.0f, 0.0f));
}

inline Vector3 Quaternion::GetRight() const
{
	return RotateVector(Vector3(0.0f, 1.0f, 0.0f));
}

inline Vector3 Quaternion::GetUp() const
{
	return RotateVector(Vector3(0.0f, 0.0f, 1.0f));
}
