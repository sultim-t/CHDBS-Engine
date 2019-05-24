#pragma once
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Math/Matrix.h>

class Compare
{
public:
	template<class Type, int Dim>
	static bool Matrices(const Matrix<Type, Dim> &matrix1, const Matrix<Type, Dim> &matrix2, float eps = 0.001f);

	template<class Type, int Dim>
	static bool Vectors(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2, float eps = 0.001f);

	static bool Quaternions(const Quaternion &a, const Quaternion &b, float eps = 0.001f);

	static bool Floats(float a, float b, float eps = 0.001f);

	template<class Type>
	static bool Arrays(int count, const Type *a, const Type *b);
};

template<class Type, int Dim>
inline bool Compare::Matrices(const Matrix<Type, Dim>& matrix, const Matrix<Type, Dim>& matrix2, float eps)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			if (Mathf::Abs(matrix(i, j) - matrix2(i, j)) > eps)
			{
				return false;
			}
		}
	}

	return true;
}

template<class Type, int Dim>
inline bool Compare::Vectors(const Vector<Type, Dim>& vec1, const Vector<Type, Dim>& vec2, float eps)
{
	for (int i = 0; i < Dim; i++)
	{
		if (Mathf::Abs(vec1[i] - vec2[i]) > eps)
		{
			return false;
		}
	}

	return true;
}

inline bool Compare::Quaternions(const Quaternion & a, const Quaternion & b, float eps)
{
	for (int i = 0; i < 4; i++)
	{
		if (Mathf::Abs(a[i] - b[i]) > eps)
		{
			return false;
		}
	}

	return true;
}

inline bool Compare::Floats(float a, float b, float eps)
{
	return Mathf::Abs(a - b) < eps;
}

template<class Type>
inline bool Compare::Arrays(int count, const Type * a, const Type * b)
{
	for (int i = 0; i < count; i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}

	return true;
}
