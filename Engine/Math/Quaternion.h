#pragma once

#include "Vector.h"
#include "Matrix.h"

// Quaternion. [0] is a free component (w)
struct Quaternion
{
private:
	float quat[4];

public:
	inline Quaternion();
	inline Quaternion(float a);
	inline Quaternion(float w, float x, float y, float z);
	// Create from euler angles in Degrees
	inline Quaternion(const Vector3 &euler);
	inline Quaternion(const Quaternion &q);
	inline Quaternion(const Vector3 &axis, float angle);
	inline Quaternion(const Matrix3 &rotationMatrix);

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

	static Quaternion Conjugate(const Quaternion &q);
	inline void Conjugate();
	static Quaternion Inverse(const Quaternion &q);
	inline void Inverse();

	// Counvert from euler in Degrees to quaternion
	static void FromEuler(const Vector3 &euler, Quaternion &quat);
	// Convert euler in Degrees to current quaternion
	inline void FromEuler(const Vector3 &euler);

	static void ToEuler(const Quaternion &quat, Vector3 &euler);
	inline const void ToEuler(Vector3 &euler) const;
	static Vector3 ToEuler(const Quaternion &quat);
	inline const Vector3 ToEuler() const;

	// Convert from axis and angle in Radians
	inline void FromAxisAngle(const Vector3 &axis, const float angle);
	// Convert to axis and angle in Radians
	inline void ToAxisAngle(Vector3 &axis, float &angle) const;

	// Creates quaternion from direction and up vector
	static Quaternion FromDirection(const Vector3 &forward, const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));

	static Matrix3 ToRotationMatrix(const Quaternion &q);
	inline Matrix3 ToRotationMatrix() const;

	inline void FromRotationMatrix(const Matrix3 &m);

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

inline Quaternion::Quaternion(float w, float x, float y, float z)
{
	quat[0] = w;
	quat[1] = x;
	quat[2] = y;
	quat[3] = z;
}

inline Quaternion::Quaternion(const Vector3& euler)
{
	FromEuler(euler, *this);
}

inline Quaternion::Quaternion(const Quaternion &q)
{
	quat[0] = q.quat[0];
	quat[1] = q.quat[1];
	quat[2] = q.quat[2];
	quat[3] = q.quat[3];
}

inline Quaternion::Quaternion(const Vector3 & axis, float angle)
{
	FromAxisAngle(axis, angle);
}

inline Quaternion::Quaternion(const Matrix3 & rotationMatrix)
{
	FromRotationMatrix(rotationMatrix);
}

inline const float Quaternion::operator[](int index) const
{
	ASSERT(index >= 0 || index < 4);
	return quat[index];
}

inline float &Quaternion::operator[](int index)
{
	ASSERT(index >= 0 || index < 4);
	return quat[index];
}

inline const Quaternion Quaternion::operator+(const Quaternion &r) const
{
	return Quaternion(quat[0] + r.quat[0], quat[1] + r.quat[1], quat[2] + r.quat[2], quat[3] + r.quat[3]);
}

inline Quaternion &Quaternion::operator+=(const Quaternion &r)
{
	quat[0] += r.quat[0];
	quat[1] += r.quat[1];
	quat[2] += r.quat[2];
	quat[3] += r.quat[3];

	return *this;
}

inline const Quaternion Quaternion::operator-(const Quaternion &r) const
{
	return Quaternion(quat[0] - r.quat[0], quat[1] - r.quat[1], quat[2] - r.quat[2], quat[3] - r.quat[3]);
}

inline Quaternion &Quaternion::operator-=(const Quaternion &r)
{
	quat[0] -= r.quat[0];
	quat[1] -= r.quat[1];
	quat[2] -= r.quat[2];
	quat[3] -= r.quat[3];

	return *this;
}

inline const Quaternion Quaternion::operator*(const Quaternion &r) const
{
	return Quaternion(
		quat[0] * r.quat[0] - quat[1] * r.quat[1] - quat[2] * r.quat[2] - quat[3] * r.quat[3],
		quat[0] * r.quat[1] + quat[1] * r.quat[0] + quat[2] * r.quat[3] - quat[3] * r.quat[2],
		quat[0] * r.quat[2] - quat[1] * r.quat[3] + quat[2] * r.quat[0] + quat[3] * r.quat[1],
		quat[0] * r.quat[3] + quat[1] * r.quat[2] - quat[2] * r.quat[1] + quat[3] * r.quat[0]);
}

inline Quaternion &Quaternion::operator*=(const Quaternion &r)
{
	*this = (*this) * r;
	return *this;
}

inline const Vector3 Quaternion::operator*(const Vector3 &v) const
{
	return RotateVector(v);
}

inline Quaternion Quaternion::Identity()
{
	// identity is quaternion with only free component (w is quat[0])
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

inline Quaternion &Quaternion::operator*=(const float scale)
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

inline Quaternion &Quaternion::operator/=(const float scale)
{
	float inv = 1.0f / scale;

	quat[0] *= inv;
	quat[1] *= inv;
	quat[2] *= inv;
	quat[3] *= inv;

	return *this;
}

inline void Quaternion::ToEuler(const Quaternion &q, Vector3& euler)
{
	using namespace Mathf;

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

const inline void Quaternion::ToEuler(Vector3 &euler) const
{
	ToEuler(*this, euler);
}

inline Vector3 Quaternion::ToEuler(const Quaternion &quat)
{
	Vector3 result;
	ToEuler(quat, result);

	return result;
}

const inline Vector3 Quaternion::ToEuler() const
{
	return ToEuler(*this);
}

inline void Quaternion::FromAxisAngle(const Vector3 &axis, float angle)
{
	using namespace Mathf;

	quat[0] = Cos(angle * 0.5f);

	float s = Sin(angle * 0.5f);
	quat[1] = axis[0] * s;
	quat[2] = axis[1] * s;
	quat[3] = axis[2] * s;
}

inline void Quaternion::ToAxisAngle(Vector3 &axis, float &angle) const
{
	using namespace Mathf;

	angle = 2.0f * ACos(quat[0]);

	float s = Sqrt(1.0f - quat[0] * quat[0]);

	if (Abs(s) >= 0.0001f)
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

inline Quaternion Quaternion::FromDirection(const Vector3 & f, const Vector3 & u)
{
	using namespace Mathf;

	Vector3 forward = f.GetNormalized();
	Vector3 right = (Vector3::Cross(u, forward)).GetNormalized();
	Vector3 up = Vector3::Cross(forward, right);
	float m00 = right[0];
	float m01 = right[1];
	float m02 = right[2];
	float m10 = up[0];
	float m11 = up[1];
	float m12 = up[2];
	float m20 = forward[0];
	float m21 = forward[1];
	float m22 = forward[2];

	float num8 = (m00 + m11) + m22;
	Quaternion quaternion;

	if (num8 > 0.0f)
	{
		float num = Sqrt(num8 + 1.0f);
		quaternion[0] = num * 0.5f;
		num = 0.5f / num;
		quaternion[1] = (m12 - m21) * num;
		quaternion[2] = (m20 - m02) * num;
		quaternion[3] = (m01 - m10) * num;
		return quaternion;
	}
	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = Sqrt(((1.0f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		quaternion[1] = 0.5f * num7;
		quaternion[2] = (m01 + m10) * num4;
		quaternion[3] = (m02 + m20) * num4;
		quaternion[0] = (m12 - m21) * num4;
		return quaternion;
	}
	if (m11 > m22)
	{
		float num6 = Sqrt(((1.0f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		quaternion[1] = (m10 + m01) * num3;
		quaternion[2] = 0.5f * num6;
		quaternion[3] = (m21 + m12) * num3;
		quaternion[0] = (m20 - m02) * num3;
		return quaternion;
	}
	float num5 = Sqrt(((1.0f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	quaternion[1] = (m20 + m02) * num2;
	quaternion[2] = (m21 + m12) * num2;
	quaternion[3] = 0.5f * num5;
	quaternion[0] = (m01 - m10) * num2;
	return quaternion;
}

inline Matrix3 Quaternion::ToRotationMatrix(const Quaternion & q)
{
	using namespace Mathf;

	Vector3 axis;
	float angle;

	q.ToAxisAngle(axis, angle);
	axis.Normalize();

	float c = Cos(angle);
	float s = Sin(angle);

	Matrix3 rotate;
	Vector3 vec = axis * (1 - c);

	rotate(0, 0) = c + vec[0] * axis[0];
	rotate(0, 1) = vec[0] * axis[1] + s * axis[2];
	rotate(0, 2) = vec[0] * axis[2] - s * axis[1];

	rotate(1, 0) = vec[1] * axis[0] - s * axis[2];
	rotate(1, 1) = c + vec[1] * axis[1];
	rotate(1, 2) = vec[1] * axis[2] + s * axis[0];

	rotate(2, 0) = vec[2] * axis[0] + s * axis[1];
	rotate(2, 1) = vec[2] * axis[1] - s * axis[0];
	rotate(2, 2) = c + vec[2] * axis[2];

	return rotate;
}

inline Matrix3 Quaternion::ToRotationMatrix() const
{
	return ToRotationMatrix(*this);
}

inline void Quaternion::FromRotationMatrix(const Matrix3 &m)
{
	using namespace Mathf;

	/*float w = Sqrt(1.0f + m(0,0) + m(1, 1) + m(2, 2)) / 2.0f;
	float w4 = 4.0f * w;

	this->quat[0] = w;
	this->quat[1] = (-m(2, 1) + m(1, 2)) / w4; // x
	this->quat[2] = (-m(0, 2) + m(2, 0)) / w4; // y
	this->quat[3] = (-m(1, 0) + m(0, 1)) / w4; // z*/

	float trace = m(0, 0) + m(1, 1) + m(2, 2);

	if (trace > 0)
	{
		float s = Sqrt(trace + 1.0f) * 2.0f;
		
		// Note: engine is using transposed matrices
		this->quat[0] = 0.25f * s;
		this->quat[1] = (-m(2, 1) + m(1, 2)) / s; // x
		this->quat[2] = (-m(0, 2) + m(2, 0)) / s; // y
		this->quat[3] = (-m(1, 0) + m(0, 1)) / s; // z
	}
	else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2))
	{
		float s = Sqrt(1.0f + m(0, 0) - m(1, 1) - m(2, 2)) * 2.0f;

		// Note: engine is using transposed matrices
		this->quat[0] = (-m(2, 1) + m(1, 2)) / s;
		this->quat[1] = 0.25f * s;
		this->quat[2] = (m(0, 1) + m(1, 0)) / s;
		this->quat[3] = (m(0, 2) + m(2, 0)) / s;
	}
	else if (m(1, 1) > m(2, 2))
	{
		float s = Sqrt(1.0f + m(1, 1) - m(0, 0) - m(2, 2)) * 2.0f;

		// Note: engine is using transposed matrices
		this->quat[0] = (-m(0, 2) + m(2, 0)) / s;
		this->quat[1] = (m(0, 1) + m(1, 0)) / s;
		this->quat[2] = 0.25f * s;
		this->quat[3] = (m(1, 2) + m(2, 1)) / s;
	}
	else
	{
		float s = Sqrt(1.0f + m(2, 2) - m(0, 0) - m(1, 1)) * 2.0f;

		// Note: engine is using transposed matrices
		this->quat[0] = (-m(1, 0) + m(0, 1)) / s;
		this->quat[1] = (m(0, 2) + m(2, 0)) / s;
		this->quat[2] = (m(1, 2) + m(2, 1)) / s;
		this->quat[3] = 0.25f * s;
	}
}

inline float Quaternion::Dot(const Quaternion &a, const Quaternion &b)
{
	return a.quat[0] * b.quat[0] + a.quat[1] * b.quat[1] + a.quat[2] * b.quat[2] + a.quat[3] * b.quat[3];
}

inline void Quaternion::Normalize()
{
	Vector4 q = Vector4(quat[0], quat[1], quat[2], quat[3]);
	q.Normalize();

	quat[0] = q[0];
	quat[1] = q[1];
	quat[2] = q[2];
	quat[3] = q[3];
}

inline Quaternion Quaternion::Conjugate(const Quaternion & q)
{
	// free component isn't affected
	return Quaternion(q.quat[0], -q.quat[1], -q.quat[2], -q.quat[3]);
}

inline void Quaternion::Conjugate()
{
	*this = Conjugate(*this);
}

inline Quaternion Quaternion::Inverse(const Quaternion & q)
{
	// inversed means normalized conjugate quaternion
	return Conjugate(q) / (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
}

inline void Quaternion::Inverse()
{
	*this = Inverse(*this);
}

inline void Quaternion::FromEuler(const Vector3& euler, Quaternion &q)
{
	using namespace Mathf;

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

inline Quaternion Quaternion::Lerp(const Quaternion &a, const Quaternion &b, const float t)
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

inline Quaternion Quaternion::Slerp(const Quaternion &a, const Quaternion &b, const float t)
{
	using namespace Mathf;

	Quaternion q;

	float it = 1 - t;

	float dot = a.quat[1] * b.quat[1] + a.quat[2] * b.quat[2] + a.quat[3] * b.quat[3] + a.quat[0] * b.quat[0];
	
	Quaternion nb = b;
	if (dot < 0.0f)
	{
		nb = b * (-1.0f);
		dot = -dot;
	}
	
	float theta0 = ACos(dot);
	float theta = theta0 * t;

	float sinTheta = Sin(theta);
	float sinTheta0 = Sin(theta0);

	float la = Cos(theta) - dot * sinTheta / sinTheta0;
	float lb = sinTheta / sinTheta0;

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

	const Vector3 q = Vector3(quat[1], quat[2], quat[3]);
	const Vector3 cross = Vector3::Cross(q, v) * 2.0f;
	
	result = v + (cross * quat[0]) + Vector3::Cross(q, cross);
	
	// result = ToRotationMatrix() * v;

	return result;
}

inline Vector3 Quaternion::GetForward() const
{
	return RotateVector(Vector3(0.0f, 0.0f, 1.0f));
}

inline Vector3 Quaternion::GetRight() const
{
	return RotateVector(Vector3(1.0f, 0.0f, 0.0f));
}

inline Vector3 Quaternion::GetUp() const
{
	return RotateVector(Vector3(0.0f, 1.0f, 0.0f));
}
