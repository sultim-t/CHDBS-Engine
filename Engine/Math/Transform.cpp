#include "Transform.h"

Transform::Transform()
{
	Reset();
}

Transform::Transform(const Vector3& pos, const Vector3& euler, const Vector3 &s)
{
	position = pos;
	quat.FromEuler(euler);
	scale = s;
}

Transform::Transform(const Vector3& pos, const Quaternion &q, const Vector3 &s)
{
	position = pos;
	quat = q;
	scale = s;
}

void Transform::Translate(const Vector3& vec)
{
	position += vec;
}

void Transform::Rotate(const Vector3& euler)
{
	Rotate(Quaternion(euler));
}

void Transform::Rotate(const Quaternion &q)
{
	quat *= q;
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
}

const Vector3 &Transform::GetPosition() const
{
	return position;
}

Vector3 &Transform::GetPosition()
{
	return position;
}

const Vector3 &Transform::GetScale() const
{
	return scale;
}

Vector3 &Transform::GetScale()
{
	return scale;
}

const Vector3 Transform::GetEuler() const
{
	return quat.ToEuler();
}

const Quaternion &Transform::GetRotation() const
{
	return quat;
}

Quaternion &Transform::GetRotation()
{
	return quat;
}

// Setters

void Transform::SetPosition(const Vector3& p)
{
	position = p;
}

void Transform::SetRotation(const Vector3& euler)
{
	quat = Quaternion(euler);
}

void Transform::SetScale(const Vector3& s)
{
	scale = s;
}

void Transform::SetRotation(Quaternion &q)
{
	quat = q;
}

const Vector3 Transform::GetForward() const
{
	return quat.GetForward();
}

const Vector3 Transform::GetRight() const
{
	return quat.GetRight();
}

const Vector3 Transform::GetUp() const
{
	return quat.GetUp();
}

const void Transform::GetFRU(Vector3 &f, Vector3 &r, Vector3 &u) const
{
	f = quat.GetForward();
	r = quat.GetRight();
	u = quat.GetUp();
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

// right handed
Matrix4 Transform::LookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up)
{
	Matrix4 mat;

	Vector3 f, s, u;

	// front
	f = target - position;
	f.Normalize();

	// left
	s = Vector3::Cross(f, up);
	s.Normalize();

	// up
	u = Vector3::Cross(s, f);

	for (int i = 0; i < 3; i++)
	{
		mat(i, 0) = s[i];
		mat(i, 1) = u[i];
		mat(i, 2) = -f[i];
	}

	mat(3, 0) = -Vector3::Dot(s, position);
	mat(3, 1) = -Vector3::Dot(u, position);
	mat(3, 2) = Vector3::Dot(f, position);

	mat(0, 3) = 0.0f;
	mat(1, 3) = 0.0f;
	mat(2, 3) = 0.0f;
	mat(3, 3) = 1.0f;

	return mat;
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


// left handed lookat
/*
		Matrix4 mat;

		Vector3 f, s, u;

		// front
		f = target - position;
		f.Normalize();

		// left
		s = Vector3::Cross(up, f);
		s.Normalize();

		// up
		u = Vector3::Cross(f, s);

		for (int i = 0; i < 3; i++)
		{
			mat(i, 0) = s[i];
			mat(i, 1) = u[i];
			mat(i, 2) = f[i];
		}

		mat(3, 0) = -Vector3::Dot(s, position);
		mat(3, 1) = -Vector3::Dot(u, position);
		mat(3, 2) = -Vector3::Dot(f, position);

		mat(0, 3) = 0.0f;
		mat(1, 3) = 0.0f;
		mat(2, 3) = 0.0f;
		mat(3, 3) = 1.0f;

		return mat;
*/