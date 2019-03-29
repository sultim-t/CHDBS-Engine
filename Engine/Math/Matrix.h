#pragma once

#include "Vector.h"

// square matrix
template <class Type, int Dim>
struct Matrix
{
private:
	Type matrix[Dim][Dim];

public:
	Matrix();
	Matrix(const Type x);
	Matrix(const Type x, const bool diagonal);
	Matrix(const int size, const Type *arr);

	inline Type &operator()(int row, int column);
	const Type &operator()(int row, int column) const;
	inline Vector<Type, Dim> operator[](int index) const;

	// matrix-matrix opertaions
	inline Matrix<Type, Dim> operator+(const Matrix<Type, Dim> &matrix2) const;
	inline Matrix<Type, Dim> &operator+=(const Matrix<Type, Dim> &matrix2);
	inline Matrix<Type, Dim> operator-(const Matrix<Type, Dim> &matrix2) const;
	inline Matrix<Type, Dim> &operator-=(const Matrix<Type, Dim> &matrix2);
	inline Matrix<Type, Dim> operator*(const Matrix<Type, Dim> &matrix2) const;
	inline Matrix<Type, Dim> &operator*=(const Matrix<Type, Dim> &matrix2);

	// matrix-vector operations
	inline Vector<Type, Dim> operator*(const Vector<Type, Dim> &vec) const;

	// matrix-scalar opertaions
	inline Matrix<Type, Dim> operator*(const Type scalar) const;
	inline Matrix<Type, Dim> &operator*=(const Type scalar);
	inline Matrix<Type, Dim> operator/(const Type scalar) const;
	inline Matrix<Type, Dim> &operator/=(const Type scalar);

	inline Matrix<Type, Dim> GetTransposed() const;

	inline const Type *ToArray() const;
	inline void SetRow(int i, const Vector<Type, Dim> &vec);
};

template <class Type, int Dim>
inline Matrix<Type, Dim>::Matrix()
{ }

template<class Type, int Dim>
inline Matrix<Type, Dim>::Matrix(const Type x)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			(*this)(i, j) = x;
		}
	}
}

template<class Type, int Dim>
inline Matrix<Type, Dim>::Matrix(const Type x, const bool diagonal)
{
	if (diagonal)
	{
		for (int i = 0; i < Dim; i++)
		{
			for (int j = 0; j < Dim; j++)
			{
				(*this)(i, j) = i == j ? x : 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < Dim; i++)
		{
			for (int j = 0; j < Dim; j++)
			{
				(*this)(i, j) = x;
			}
		}
	}
}

template<class Type, int Dim>
inline Matrix<Type, Dim>::Matrix(const int size, const Type * arr)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			int index = i*Dim + j;
			(*this)(i, j) = index < size ? arr[index] : 0;
		}
	}
}

/*template<class Type, int Dim>
inline Matrix<Type, Dim>::Matrix(const Type arr[Dim * Dim])
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			(*this)(i, j) = arr[i * Dim + j];
		}
	}
}*/

/*template<class Type, int Dim>
inline Matrix<Type, Dim>::Matrix(const Type arr[Dim][Dim])
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			(*this)(i, j) = arr[i][j];
		}
	}
}*/

template<class Type, int Dim>
inline Type &Matrix<Type, Dim>::operator()(int row, int column)
{
	ASSERT(row >= 0 && row < Dim && column >= 0 && column < Dim);
	return matrix[row][column];
}

template<class Type, int Dim>
inline const Type &Matrix<Type, Dim>::operator()(int row, int column) const
{
	ASSERT(row >= 0 && row < Dim && column >= 0 && column < Dim);
	return matrix[row][column];
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::operator+(const Matrix<Type, Dim>& matrix2) const
{
	return Matrix<Type, Dim>(*this) += matrix2;
}

template<class Type, int Dim>
inline Matrix<Type, Dim>& Matrix<Type, Dim>::operator+=(const Matrix<Type, Dim>& matrix2)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			(*this)(i, j) += matrix2(i, j);
		}
	}
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::operator-(const Matrix<Type, Dim>& matrix2) const
{
	return Matrix<Type, Dim>(*this) -= matrix2;
}

template<class Type, int Dim>
inline Matrix<Type, Dim>& Matrix<Type, Dim>::operator-=(const Matrix<Type, Dim>& matrix2)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			(*this)(i, j) -= matrix2(i, j);
		}
	}
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::operator*(const Matrix<Type, Dim>& matrix2) const
{
	Matrix<Type, Dim> result;

	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			result(i, j) = (Type)0;

			for (int k = 0; k < Dim; k++)
			{
				result(i, j) += (*this)(i, k) * matrix2(k, j);
			}
		}
	}

	return result;
}

template<class Type, int Dim>
inline Matrix<Type, Dim>& Matrix<Type, Dim>::operator*=(const Matrix<Type, Dim>& matrix2)
{
	*this = *this * matrix2;
	return *this;
}

template<class Type, int Dim>
inline Vector<Type, Dim> Matrix<Type, Dim>::operator*(const Vector<Type, Dim>& vec) const
{
	Vector<Type, Dim> result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = (Type)0;
		for (int j = 0; j < Dim; j++)
		{
			result[i] += matrix[i][j] * vec[j];
		}
	}

	return result;
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::operator*(const Type scalar) const
{
	return Matrix<Type, Dim>(*this) *= scalar;
}

template<class Type, int Dim>
inline Matrix<Type, Dim>& Matrix<Type, Dim>::operator*=(const Type scalar)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			(*this)(i, j) *= scalar;
		}
	}

	return *this;
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::operator/(const Type scalar) const
{
	return Matrix<Type, Dim>(*this) *= (1 / scalar);
}

template<class Type, int Dim>
inline Matrix<Type, Dim>& Matrix<Type, Dim>::operator/=(const Type scalar)
{
	(*this) *= (1 / scalar);
	return *this;
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::GetTransposed() const
{
	Matrix<Type, Dim> result;

	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			result(i, j) = this->matrix[j][i];
		}
	}

	return result;
}

template<class Type, int Dim>
inline const Type *Matrix<Type, Dim>::ToArray() const
{
	return &matrix[0][0];
}

template<class Type, int Dim>
inline void Matrix<Type, Dim>::SetRow(int row, const Vector<Type, Dim>& vec)
{
	ASSERT(row < Dim);
	
	for (int i = 0; i < Dim; i++)
	{
		matrix[row][i] = vec[i];
	}
}

/*
// for float matrices: init as unit

Matrix2::Matrix2()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			(*this)(i, j) = i != j ? 0.0f : 1.0f;
		}
	}
}

Matrix<float, 3>::Matrix<float, 3>()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			(*this)(i, j) = i != j ? 0.0f : 1.0f;
		}
	}
}

Matrix<float, 4>::Matrix<float, 4>()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*this)(i, j) = i != j ? 0.0f : 1.0f;
		}
	}
}*/

// for 3x3 float matrices

inline Matrix<float, 3> Matrix<float, 3>::operator*(const Matrix<float, 3> &matrix2) const
{
	Matrix<float, 3> result;

	result(0, 0) = (*this)(0, 0) * matrix2(0, 0) + (*this)(0, 1) * matrix2(1, 0) + (*this)(0, 2) * matrix2(2, 0);
	result(0, 1) = (*this)(0, 0) * matrix2(0, 1) + (*this)(0, 1) * matrix2(1, 1) + (*this)(0, 2) * matrix2(2, 1);
	result(0, 2) = (*this)(0, 0) * matrix2(0, 2) + (*this)(0, 1) * matrix2(1, 2) + (*this)(0, 2) * matrix2(2, 2);

	result(1, 0) = (*this)(1, 0) * matrix2(0, 0) + (*this)(1, 1) * matrix2(1, 0) + (*this)(1, 2) * matrix2(2, 0);
	result(1, 1) = (*this)(1, 0) * matrix2(0, 1) + (*this)(1, 1) * matrix2(1, 1) + (*this)(1, 2) * matrix2(2, 1);
	result(1, 2) = (*this)(1, 0) * matrix2(0, 2) + (*this)(1, 1) * matrix2(1, 2) + (*this)(1, 2) * matrix2(2, 2);

	result(2, 0) = (*this)(2, 0) * matrix2(0, 0) + (*this)(2, 1) * matrix2(1, 0) + (*this)(2, 2) * matrix2(2, 0);
	result(2, 1) = (*this)(2, 0) * matrix2(0, 1) + (*this)(2, 1) * matrix2(1, 1) + (*this)(2, 2) * matrix2(2, 1);
	result(2, 2) = (*this)(2, 0) * matrix2(0, 2) + (*this)(2, 1) * matrix2(1, 2) + (*this)(2, 2) * matrix2(2, 2);

	return result;
}

// for 3x3 double matrices

inline Matrix<double, 3> Matrix<double, 3>::operator*(const Matrix<double, 3> &matrix2) const
{
	Matrix<double, 3> result;

	result(0, 0) = (*this)(0, 0) * matrix2(0, 0) + (*this)(0, 1) * matrix2(1, 0) + (*this)(0, 2) * matrix2(2, 0);
	result(0, 1) = (*this)(0, 0) * matrix2(0, 1) + (*this)(0, 1) * matrix2(1, 1) + (*this)(0, 2) * matrix2(2, 1);
	result(0, 2) = (*this)(0, 0) * matrix2(0, 2) + (*this)(0, 1) * matrix2(1, 2) + (*this)(0, 2) * matrix2(2, 2);

	result(1, 0) = (*this)(1, 0) * matrix2(0, 0) + (*this)(1, 1) * matrix2(1, 0) + (*this)(1, 2) * matrix2(2, 0);
	result(1, 1) = (*this)(1, 0) * matrix2(0, 1) + (*this)(1, 1) * matrix2(1, 1) + (*this)(1, 2) * matrix2(2, 1);
	result(1, 2) = (*this)(1, 0) * matrix2(0, 2) + (*this)(1, 1) * matrix2(1, 2) + (*this)(1, 2) * matrix2(2, 2);

	result(2, 0) = (*this)(2, 0) * matrix2(0, 0) + (*this)(2, 1) * matrix2(1, 0) + (*this)(2, 2) * matrix2(2, 0);
	result(2, 1) = (*this)(2, 0) * matrix2(0, 1) + (*this)(2, 1) * matrix2(1, 1) + (*this)(2, 2) * matrix2(2, 1);
	result(2, 2) = (*this)(2, 0) * matrix2(0, 2) + (*this)(2, 1) * matrix2(1, 2) + (*this)(2, 2) * matrix2(2, 2);

	return result;
}

template<class Type, int Dim>
inline Vector<Type, Dim> Matrix<Type, Dim>::operator[](int index) const
{
	Vector<Type, Dim> vec;

	for (int i = 0; i < Dim; i++)
	{
		vec[i] = (*this)(index, i);
	}

	return vec;
}