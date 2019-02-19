#pragma once

#include "Vector.h"

struct Quaternion
{
private:
	float quat[4];

public:
	inline Quaternion();
	inline Quaternion(float a);
	inline Quaternion(float x, float y, float z, float w);
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

	static float Dot(const Quaternion &a, const Quaternion &b);
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
	inline void FromAxisAngle(const Vector3 &axis, const float angle);
	// Convert to axis and angle in Radians
	inline void ToAxisAngle(Vector3 &axis, float &angle) const;

	// Interpolates between a and b, normalizes the result, t is clamped to [0,1]
	static Quaternion Lerp(const Quaternion &a, const Quaternion &b, const float t);
	// Spherically interpolates between a and b, t is clamped to [0,1]
	static Quaternion Slerp(const Quaternion &a, const Quaternion &b, const float t);

	inline Vector3 RotateVector(const Vector3 &v) const;

	inline Vector3 GetForward() const;
	inline Vector3 GetRight() const;
	inline Vector3 GetUp() const;
};

inline Quaternion::Quaternion()
{
}

inline Quaternion::Quaternion(float a)
{
	quat[0] = quat[1] = quat[2] = quat[3] = a;
}

inline Quaternion::Quaternion(float x, float y, float z, float w)
{
	quat[0] = x;
	quat[1] = y;
	quat[2] = z;
	quat[3] = w;
}

inline Quaternion::Quaternion(const Vector3& euler)
{
	FromEuler(euler, *this);
}

inline Quaternion::Quaternion(const Quaternion & q)
{
	quat[0] = q.quat[0];
	quat[1] = q.quat[1];
	quat[2] = q.quat[2];
	quat[3] = q.quat[3];
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
	return Quaternion(quat[0] + r.quat[0], quat[1] + r.quat[1], quat[2] + r.quat[2], quat[3] + r.quat[3]);
}

inline Quaternion & Quaternion::operator+=(const Quaternion & r)
{
	quat[0] += r.quat[0];
	quat[1] += r.quat[1];
	quat[2] += r.quat[2];
	quat[3] += r.quat[3];

	return *this;
}

inline const Quaternion Quaternion::operator-(const Quaternion & r) const
{
	return Quaternion(quat[0] - r.quat[0], quat[1] - r.quat[1], quat[2] - r.quat[2], quat[3] - r.quat[3]);
}

inline Quaternion & Quaternion::operator-=(const Quaternion & r)
{
	quat[0] -= r.quat[0];
	quat[1] -= r.quat[1];
	quat[2] -= r.quat[2];
	quat[3] -= r.quat[3];

	return *this;
}

inline const Quaternion Quaternion::operator*(const Quaternion & r) const
{
	return Quaternion(
		quat[0] * r.quat[0] - quat[1] * r.quat[1] - quat[2] * r.quat[2] - quat[3] * r.quat[3],
		quat[0] * r.quat[1] + quat[1] * r.quat[0] + quat[2] * r.quat[3] - quat[3] * r.quat[2],
		quat[0] * r.quat[2] - quat[1] * r.quat[3] + quat[2] * r.quat[0] + quat[3] * r.quat[1],
		quat[0] * r.quat[3] + quat[1] * r.quat[2] - quat[2] * r.quat[1] + quat[3] * r.quat[0]);
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
		quat[0] * scale,
		quat[1] * scale,
		quat[2] * scale,
		quat[3] * scale);
}

inline Quaternion & Quaternion::operator*=(const float scale)
{
	quat[0] *= scale;
	quat[1] *= scale;
	quat[2] *= scale;
	quat[3] *= scale;

	return *this;
}

inline const Quaternion Quaternion::operator/(const float scale) const
{
	float inv = 1.0f / scale;

	return Quaternion(
		quat[0] * inv,
		quat[1] * inv,
		quat[2] * inv,
		quat[3] * inv);
}

inline Quaternion & Quaternion::operator/=(const float scale)
{
	float inv = 1.0f / scale;

	quat[0] *= inv;
	quat[1] *= inv;
	quat[2] *= inv;
	quat[3] *= inv;

	return *this;
}

inline void Quaternion::ToEuler(const Quaternion & q, Vector3& euler)
{
	// roll
	float sinr = +2.0f * (q.quat[0] * q.quat[1] + q.quat[2] * q.quat[3]);
	float cosr = +1.0f - 2.0f * (q.quat[1] * q.quat[1] + q.quat[2] * q.quat[2]);
	euler[0] = RAD2DEG(ATan2(sinr, cosr));

	// pitch
	float sinp = +2.0f * (q.quat[0] * q.quat[2] - q.quat[3] * q.quat[1]);
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
	float siny = +2.0f * (q.quat[0] * q.quat[3] + q.quat[1] * q.quat[2]);
	float cosy = +1.0f - 2.0f * (q.quat[2] * q.quat[2] + q.quat[3] * q.quat[3]);
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
	quat[0] = Cos(angle * 0.5f);

	float s = Sin(angle * 0.5f);
	quat[1] = axis[0] * s;
	quat[2] = axis[1] * s;
	quat[3] = axis[2] * s;
}

inline void Quaternion::ToAxisAngle(Vector3 & axis, float & angle) const
{
	angle = 2 * ACos(quat[0]);

	float s = Sqrt(1 - quat[0] * quat[0]);

	if (ABS(s) >= 0.001f)
	{
		axis[0] = quat[1] / s;
		axis[1] = quat[2] / s;
		axis[2] = quat[3] / s;
	}
	else
	{
		// angle is zero
		axis[0] = 1.0f;
		axis[1] = 0.0f;
		axis[2] = 0.0f;
	}
}

inline float Quaternion::Dot(const Quaternion & a, const Quaternion & b)
{
	return a.quat[0] * b.quat[0] + a.quat[1] * b.quat[1] + a.quat[2] * b.quat[2] + a.quat[3] * b.quat[3];
}

inline void Quaternion::Normalize()
{
	Vector4 q = (quat[0], quat[1], quat[2], quat[3]);
	q.Normalize();

	quat[0] = q[0];
	quat[1] = q[1];
	quat[2] = q[2];
	quat[3] = q[3];
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

	q.quat[0] = cy * cr * cp + sy * sr * sp;
	q.quat[1] = cy * sr * cp - sy * cr * sp;
	q.quat[2] = cy * cr * sp + sy * sr * cp;
	q.quat[3] = sy * cr * cp - cy * sr * sp;
}

inline void Quaternion::FromEuler(const Vector3& euler)
{
	FromEuler(euler, *this);
}

inline Quaternion Quaternion::Lerp(const Quaternion & a, const Quaternion & b, const float t)
{
	Quaternion q;

	float it = 1 - t;

	q.quat[1] = it * a.quat[1] + t * b.quat[1];
	q.quat[2] = it * a.quat[2] + t * b.quat[2];
	q.quat[3] = it * a.quat[3] + t * b.quat[3];
	q.quat[0] = it * a.quat[0] + t * b.quat[0];

	q.Normalize();

	return q;
}

inline Quaternion Quaternion::Slerp(const Quaternion & a, const Quaternion & b, const float t)
{
	Quaternion q;

	float it = 1 - t;

	float theta = ACos(a.quat[1]*b.quat[1] + a.quat[2]*b.quat[2] + a.quat[3]*b.quat[3] + a.quat[0]*b.quat[0]);
	float s = Sin(theta);

	float la = Sin(it * theta) / s;
	float lb = Sin(t * theta) / s;

	q.quat[0] = la * a.quat[0] + lb * b.quat[0];
	q.quat[1] = la * a.quat[1] + lb * b.quat[1];
	q.quat[2] = la * a.quat[2] + lb * b.quat[2];
	q.quat[3] = la * a.quat[3] + lb * b.quat[3];

	q.Normalize();

	return q;
}

inline Vector3 Quaternion::RotateVector(const Vector3 &v) const
{
	Vector3 result;

	const Vector3 q = Vector3(quat[0], quat[1], quat[2]);
	const Vector3 cross = Vector3::Cross(q, v) * 2.0f;
	
	result = v + (cross * quat[3]) + Vector3::Cross(q, cross);

	// result = q * 2.0f * Vector3::Dot(q, v)
	//	+ v * (quat[3] * quat[3] - Vector3::Dot(q, q))
	//	+ cross;

	return result;
}

inline Vector3 Quaternion::GetForward() const
{
	return RotateVector(Vector3(1.0f, 0.0f, 0.0f));
}

inline Vector3 Quaternion::GetRight() const
{
	return -RotateVector(Vector3(0.0f, 0.0f, 1.0f));
}

inline Vector3 Quaternion::GetUp() const
{
	return -RotateVector(Vector3(0.0f, 1.0f, 0.0f));
}
