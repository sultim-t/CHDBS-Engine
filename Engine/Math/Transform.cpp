#include "Transform.h"

Transform::Transform()
{
	Reset();
}

Transform::Transform(const Vector3 &pos, const Vector3 &euler, const Vector3 &s)
{
	position = pos;
	quat.FromEuler(euler);
	scale = s;
}

Transform::Transform(const Vector3 &pos, const Quaternion &q, const Vector3 &s)
{
	position = pos;
	quat = q;
	scale = s;
}

Transform::Transform(const Matrix4 &m)
{
	FromMatrix(m);
}

void Transform::Translate(const Vector3 &vec)
{
	position += vec;
}

void Transform::Rotate(const Vector3 &euler)
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

Vector3 Transform::DirectionFromLocal(const Vector3 &dir) const
{
	// rotate dir by current rotation
	return quat * dir;
}

Vector3 Transform::DirectionFromWorld(const Vector3 &dir) const
{
	return Quaternion::Inverse(quat) * dir;
}

Vector3 Transform::PointFromLocal(const Vector3 &vec) const
{
	return quat * (Vector3::Scale(vec, scale)) + position;
}

Vector3 Transform::PointFromWorld(const Vector3 &vec) const
{
	ASSERT(scale[0] != 0.0f && scale[1] != 0.0f && scale[2] != 0.0f);

	Vector3 result = vec - position;
	
	// unrotate
	result = Quaternion::Inverse(quat) * result;

	// inverse scale
	Vector3 invScale = Vector3(1 / scale[0], 1 / scale[1], 1 / scale[2]);

	// unscale
	result = Vector3::Scale(result, invScale);

	return result;
}

Matrix4 Transform::TranslateMatrix(const Matrix4 &mat, const Vector3 &vec)
{
	Matrix4 result(mat);

	result.SetRow(3, mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3]);
	return result;
}

Matrix4 Transform::RotateMatrix(const Matrix4& mat, const Vector3 &inaxis, const float angle)
{
	float c = Cos(angle);
	float s = Sin(angle);

	Matrix3 rotate;
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

	Matrix4 result;

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

void Transform::FromMatrix(const Matrix4 &m)
{
	// decompose position
	position = Vector3(m(3, 0), m(3, 1), m(3, 2));

	// decompose scale
	float s[3];
	s[0] = Vector3(m(0, 0), m(0, 1), m(0, 2)).Length();
	s[1] = Vector3(m(1, 0), m(1, 1), m(1, 2)).Length();
	s[2] = Vector3(m(2, 0), m(2, 1), m(2, 2)).Length();
	scale = Vector3(s[0], s[1], s[2]);
	
	// decompose rotaion matrix
	Matrix3 rotationMatrix;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMatrix(i, j) = m(i, j) / s[i];
		}
	}

	// convert to quaternion
	quat = Quaternion(rotationMatrix);
}

Vector3 Transform::DecomposePosition(const Matrix4 & m)
{
	return Vector3(m(3, 0), m(3, 1), m(3, 2));
}

Quaternion Transform::DecomposeRotation(const Matrix4 & m)
{
	// decompose scale
	float s[3];
	s[0] = Vector3(m(0, 0), m(0, 1), m(0, 2)).Length();
	s[1] = Vector3(m(1, 0), m(1, 1), m(1, 2)).Length();
	s[2] = Vector3(m(2, 0), m(2, 1), m(2, 2)).Length();

	// decompose rotaion matrix
	Matrix3 rotationMatrix;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMatrix(i, j) = m(i, j) / s[i];
		}
	}

	// convert to quaternion
	return Quaternion(rotationMatrix);
}

Vector3 Transform::DecomposeScale(const Matrix4 & m)
{
	return Vector3(
		Vector3(m(0, 0), m(0, 1), m(0, 2)).Length(), 
		Vector3(m(1, 0), m(1, 1), m(1, 2)).Length(),
		Vector3(m(2, 0), m(2, 1), m(2, 2)).Length());
}