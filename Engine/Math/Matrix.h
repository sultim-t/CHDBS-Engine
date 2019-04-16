#pragma once

#include "Vector.h"

// square matrix
template <class Type, int Dim>
struct Matrix
{
private:
	Type matrix[Dim][Dim];

public:
	// Empty contrutor
	Matrix();
	// Construct matrix, all elements are set to "x"
	Matrix(const Type x);
	// Construct matrix
	Matrix(const Type x, bool diagonal);
	// Construct matrix from raw array
	Matrix(int size, const Type *arr);

	// Get element in this matrix
	inline Type &operator()(int row, int column);
	// Get element in this matrix
	inline const Type &operator()(int row, int column) const;
	// Get copy of vector of this matrix
	inline Vector<Type, Dim> operator[](int index) const;

	// matrix-matrix operations
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

	// Transpose this matrix
	inline void Transpose();
	// Get transposed copy of this matrix
	inline Matrix<Type, Dim> GetTransposed() const;

	// Get determinant of this matrix
	inline Type GetDeterminant() const;
	
	// Inverse this matrix
	inline void Inverse();
	// Get inversed copy of this matrix
	inline Matrix<Type, Dim> GetInversed() const;

	// Cast to array of length Dim*Dim
	inline const Type *ToArray() const;
	// Set row in this matrix
	inline void SetRow(int i, const Vector<Type, Dim> &vec);

private:
	// Get determinant of this matrix,
	// but for size n*n
	inline Type GetDeterminant(int n) const;
	// Get this matrix without given row and column
	// Size of input matrix is n*n
	// Size of output matrix is (n-1)*(n-1)
	// Result matrix is "out"
	void GetWithout(Matrix<Type, Dim> &out, int row, int column, int n) const;
	// Get copy of this matrix without given row and column
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
inline Matrix<Type, Dim>::Matrix(const Type x, bool diagonal)
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
inline Matrix<Type, Dim>::Matrix(int size, const Type * arr)
{
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			int index = i*Dim + j;
			(*this)(i, j) = index < size ? arr[index] : (Type)0;
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

	return *this;
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
	
	return *this;
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
inline void Matrix<Type, Dim>::Transpose()
{
	*this = GetTransposed();
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
inline void Matrix<Type, Dim>::GetWithout(Matrix<Type, Dim>& out, int row, int column, int n) const
{
	// iterators for result matrix
	int k = 0, l = 0;

	for (int i = 0; i < n; i++)
	{
		// row to delete
		if (i == row)
		{
			// ignore
			continue;
		}

		for (int j = 0; j < n; j++)
		{
			// column to delete
			if (j == column)
			{
				// ignore
				continue;
			}

			// copy
			out(k, l) = matrix[i][j];

			l++;
		}

		k++;
		l = 0;
	}
}

template<class Type, int Dim>
inline Type Matrix<Type, Dim>::GetDeterminant() const
{
	return GetDeterminant(Dim);
}

template<class Type, int Dim>
inline Type Matrix<Type, Dim>::GetDeterminant(int n) const
{
	if (n == 1)
	{
		// if matrix 1x1
		return matrix[0][0];
	}
	else if (n == 2)
	{
		// if matrix 2x2
		return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
	}

	Type det = (Type)0;
	Type sign = (Type)1;

	// temp matrix Dim*Dim
	// but used size is (n-1)*(n-1)
	Matrix<Type, Dim> a;

	// for each element in top row
	for (int j = 0; j < n; j++)
	{
		// get matrix without top row and j column
		// input size is n*n
		// output size is (n-1)*(n-1)
		GetWithout(a, 0, j, n);

		// calculate minor
		// for matrix a, which size is (n-1)*(n-1)
		float minor = a.GetDeterminant(n - 1);

		// add to result
		det += sign * matrix[0][j] * minor;

		sign = -sign;
	}

	return det;
}

template<class Type, int Dim>
inline void Matrix<Type, Dim>::Inverse()
{
	*this = GetInversed();
}

template<class Type, int Dim>
inline Matrix<Type, Dim> Matrix<Type, Dim>::GetInversed() const
{
	Matrix<Type, Dim> inversed;

	// 1 / det
	Type invDet = (Type)1 / this->GetDeterminant();

	if (Dim == 1)
	{
		// if matrix 1x1, then ajugate matrix is { 1 }
		// so inversed is 1/det
		inversed(0, 0) = invDet;
		return inversed;
	}
	else if (Dim == 2)
	{
		// if matrix 2x2
		inversed(0, 0) = matrix[1][1];
		inversed(0, 1) = -matrix[0][1];
		inversed(1, 0) = -matrix[1][0];
		inversed(1, 1) = matrix[0][0];
		
		// multiply by inversed determinant
		inversed *= invDet;

		return inversed;
	}

	// if matrix order > 2

	Type sign = (Type)1;

	// temp matrix Dim*Dim
	// used size is (Dim-1)*(Dim-1)
	Matrix<Type, Dim> temp;

	// get adjugate matrix
	for (int i = 0; i < Dim; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			// load to temp
			// matrix without row "i" and column "j"
			GetWithout(temp, i, j, Dim);

			// calculate minor
			// size of temp is (Dim-1)*(Dim-1)
			float minor = temp.GetDeterminant(Dim - 1);
			
			sign = ((i + j) % 2 == 0) ? 1.0f : -1.0f;

			// adjugate matrix is transposed
			// to inverse, multiply by invDet
			inversed(j, i) = sign * minor * invDet;

			// inverse sign
			sign = -sign;
		}
	}

	return inversed;
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