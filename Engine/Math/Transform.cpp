#include "Transform.h"

//temp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
inline Matrix4 FromGLM(const glm::mat4 &m);
inline glm::mat4 ToGLM(const Matrix4 &m);

Transform::Transform()
{
	Reset();
}

Transform::Transform(const Vector3& pos, const Vector3& euler, const Vector3 &s)
{
	position = pos;
	quat.FromEuler(euler);
	scale = s;
	fruWasModified = true;
}

Transform::Transform(const Vector3& pos, const Quaternion & q, const Vector3 &s)
{
	position = pos;
	quat = q;
	scale = s;
	fruWasModified = true;
}

void Transform::Translate(const Vector3& vec)
{
	position += vec;
}

void Transform::Rotate(const Vector3& euler)
{
	Rotate(Quaternion(euler));
}

void Transform::Rotate(const Quaternion & q)
{
	quat *= q;
	fruWasModified = true;
}

void Transform::Scale(const float scale)
{
	this->scale *= scale;
}

void Transform::Scale(const Vector3& scale)
{
	for (int i = 0; i < 3; i++)
	{
		this->scale[i] *= scale[i];
	}
}

void Transform::Reset()
{
	position = (0.0f, 0.0f, 0.0f);
	quat = Quaternion::Identity();
	scale = (1.0f, 1.0f, 1.0f);

	fruWasModified = true;
}

const void Transform::GetPosition(Vector3& pos) const
{
	pos = Vector3(position);
}

const void Transform::GetEuler(Vector3& euler) const
{
	euler = quat.ToEuler();
}

const void Transform::GetScale(Vector3& s) const
{
	s = Vector3(scale);
}

const void Transform::GetRotation(Quaternion & q) const
{
	q = Quaternion(quat);
}

// Get copies

const Vector3 Transform::GetPosition() const
{
	return position;
}

const Vector3 Transform::GetScale() const
{
	return Vector3(scale);
}

const Vector3 Transform::GetEuler() const
{
	return quat.ToEuler();
}

const Quaternion Transform::GetRotation() const
{
	return Quaternion(quat);
}

// Setters

void Transform::SetPosition(Vector3& p)
{
	position = p;
}

void Transform::SetRotation(Vector3& euler)
{
	quat = Quaternion(euler);
	fruWasModified = true;
}

void Transform::SetScale(Vector3& s)
{
	scale = s;
}

void Transform::SetRotation(Quaternion & q)
{
	quat = q;
	fruWasModified = true;
}

const Vector3 Transform::GetForward() const
{
	if (!fruWasModified)
	{
		return forward;
	}

	GetFRU(forward, right, up);

	return forward;
}

/*Vector3 Transform::GetRight(Vector3 forward)
{
	return Vector3::Cross(forward, WORLDUP).GetNormalized();
}*/

const Vector3 Transform::GetRight() const
{
	if (!fruWasModified)
	{
		return right;
	}

	GetFRU(forward, right, up);
	
	return right;
}

const Vector3 Transform::GetUp() const
{
	if (!fruWasModified)
	{
		return up;
	}

	GetFRU(forward, right, up);

	return up;
}

const void Transform::GetFRU(Vector3 & f, Vector3 & r, Vector3 & u) const
{
	if (!fruWasModified)
	{
		f = Vector3(forward);
		r = Vector3(right);
		u = Vector3(up);
		return;
	}

	Euler e = quat.ToEuler();

	Vector3 t;
	t[0] = Cos(DEG2RAD(e[YAW])) * Cos(DEG2RAD(e[PITCH]));
	t[1] = Sin(DEG2RAD(e[PITCH]));
	t[2] = Sin(DEG2RAD(e[YAW])) * Cos(DEG2RAD(e[PITCH]));
	f = t.GetNormalized();

	r = Vector3::Cross(f, Vector3(0, 1, 0)).GetNormalized();
	u = Vector3::Cross(r, f).GetNormalized();

	/*f = quat * Vector3(1.0f, 0.0f, 0.0f);
	f.Normalize();
	r = quat * Vector3(0.0f, 1.0f, 0.0f);
	r.Normalize();
	u = quat * Vector3(1.0f, 0.0f, 0.0f);
	u.Normalize();*/

	fruWasModified = false;
}

const Matrix4 Transform::GetTransformMatrix() const
{
	Matrix4 result = Matrix4::Matrix(1.0f, true);

	// to axis-angle
	Vector3 axis; 
	float angle;

	quat.ToAxisAngle(axis, angle);

	result = TranslateMatrix(result, position);
	result = RotateMatrix(result, axis, angle);
	result = ScaleMatrix(result, scale);

	return result;
}

// left handed
Matrix4 Transform::LookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up)
{
	glm::vec3 eye;
	eye.x = position[0];
	eye.y = position[1];
	eye.z = position[2];

	glm::vec3 center;
	center.x = target[0];
	center.y = target[1];
	center.z = target[2];

	glm::vec3 upN;
	upN.x = up[0];
	upN.y = up[1];
	upN.z = up[2];

	glm::mat4 m = glm::lookAt(eye, center, upN);

	return FromGLM(m);

//#	if GLM_COORDINATE_SYSTEM == GLM_LEFT_HANDED
//	{
//		Matrix4 mat;
//
//		Vector3 f, s, u;
//
//		// front
//		f = target - position;
//		f.Normalize();
//
//		// left
//		s = Vector3::Cross(up, f);
//		s.Normalize();
//
//		// up
//		u = Vector3::Cross(f, s);
//
//		for (int i = 0; i < 3; i++)
//		{
//			mat(i, 0) = s[i];
//			mat(i, 1) = u[i];
//			mat(i, 2) = f[i];
//		}
//
//		mat(3, 0) = -Vector3::Dot(s, position);
//		mat(3, 1) = -Vector3::Dot(u, position);
//		mat(3, 2) = -Vector3::Dot(f, position);
//
//		for (int i = 0; i < 4; i++)
//		{
//			mat(i, 3) = 1.0f;
//		}
//
//		return mat;
//	}
//#	else
//	{
//		Matrix4 mat;
//
//		Vector3 f, s, u;
//
//		// front
//		f = target - position;
//		f.Normalize();
//
//		// left
//		s = Vector3::Cross(f, up);
//		s.Normalize();
//
//		// up
//		u = Vector3::Cross(s, f);
//
//		for (int i = 0; i < 3; i++)
//		{
//			mat(i, 0) = s[i];
//			mat(i, 1) = u[i];
//			mat(i, 2) = -f[i];
//		}
//
//		mat(3, 0) = -Vector3::Dot(s, position);
//		mat(3, 1) = -Vector3::Dot(u, position);
//		mat(3, 2) = Vector3::Dot(f, position);
//
//		for (int i = 0; i < 4; i++)
//		{
//			mat(i, 3) = 1.0f;
//		}
//
//		return mat;
//	}
//#	endif
}

Matrix4 Transform::TranslateMatrix(const Matrix4& mat, const Vector3 &vec)
{
	Matrix4 result(mat);

	result.SetRow(3, mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3]);
	return result;
}

Matrix4 Transform::RotateMatrix(const Matrix4& mat, const Vector3 &inaxis, const float angle)
{
	float c = Cos(angle);
	float s = Sin(angle);

	Matrix3 rotate = Matrix3(1.0f, true);
	Vector3 vec;

	Vector3 axis = inaxis.GetNormalized();

	vec = axis * (1 - c);

	rotate(0, 0) = c + vec[0] * axis[0];
	rotate(0, 1) = vec[0] * axis[1] + s * axis[2];
	rotate(0, 2) = vec[0] * axis[2] - s * axis[1];

	rotate(1, 0) = vec[1] * axis[0] - s * axis[2];
	rotate(1, 1) = c + vec[1] * axis[1];
	rotate(1, 2) = vec[1] * axis[2] + s * axis[0];

	rotate(2, 0) = vec[2] * axis[0] + s * axis[1];
	rotate(2, 1) = vec[2] * axis[1] - s * axis[0];
	rotate(2, 2) = c + vec[2] * axis[2];

	Matrix4 result(mat);

	result.SetRow(0, mat[0] * rotate(0, 0) + mat[1] * rotate(0, 1) + mat[2] * rotate(0, 2));
	result.SetRow(1, mat[0] * rotate(1, 0) + mat[1] * rotate(1, 1) + mat[2] * rotate(1, 2));
	result.SetRow(2, mat[0] * rotate(2, 0) + mat[1] * rotate(2, 1) + mat[2] * rotate(2, 2));
	result.SetRow(3, mat[3]);

	return result;
}

Matrix4 Transform::ScaleMatrix(const Matrix4& mat, const Vector3 &vec)
{
	Matrix4 result(mat);

	result.SetRow(0, mat[0] * vec[0]);
	result.SetRow(1, mat[1] * vec[1]);
	result.SetRow(2, mat[2] * vec[2]);
	result.SetRow(3, mat[3]);

	return result;
}


inline Matrix4 FromGLM(const glm::mat4 &m)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result(i, j) = m[i][j];
		}
	}

	return result;
}

inline glm::mat4 ToGLM(const Matrix4 &m)
{
	glm::mat4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = m(i,j);
		}
	}

	return result;
}