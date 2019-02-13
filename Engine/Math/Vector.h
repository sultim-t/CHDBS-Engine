#pragma once

#include "BasicMath.h"

template <class Type, int Dim>
struct Vector
{
private:
	Type vector[Dim];
public:
	// Empty constructor
	inline Vector();
	// Init all dimensions with 'x' value
	inline Vector(const Type x);
	// Init first 2 dimensions
	inline Vector(const Type x, const Type y);
	// Init first 3 dimensions
	inline Vector(const Type x, const Type y, const Type z);
	// Init first 4 dimensions
	inline Vector(const Type x, const Type y, const Type z, const Type w);

	inline Type& operator[](int index);
	inline const Type operator[](int index) const;
	inline Vector<Type, Dim>operator+(const Vector<Type, Dim>in) const;
	inline Vector<Type, Dim>&operator+=(const Vector<Type, Dim>in);
	inline Vector<Type, Dim>operator-() const;
	inline Vector<Type, Dim>operator-(const Vector<Type, Dim>in) const;
	inline Vector<Type, Dim>&operator-=(const Vector<Type, Dim>in);
	inline Vector<Type, Dim>operator*(const Type scale) const;
	inline Vector<Type, Dim>&operator*=(const Type scale);
	inline bool operator==(const Vector<Type, Dim>vec);
	inline bool operator==(const Vector<Type, Dim>vec) const;
	inline bool operator!=(const Vector<Type, Dim>vec);

	static void Copy(const Vector<Type, Dim> & in, Vector<Type, Dim> & out);
	static bool ApproximateCompare(const Vector<Type, Dim> & vec1, const Vector<Type, Dim> & vec2);
	// Length of vec
	static Type Length(const Vector<Type, Dim> & vec);
	// Sqr length vec
	static Type LengthSqr(const Vector<Type, Dim> & vec);
	// Manhattan norm
	static Type ManhattanNorm(const Vector<Type, Dim> & vec);
	// Distance between vec1 and vec2
	static Type Distance(const Vector<Type, Dim> & vec1, const Vector<Type, Dim> & vec2);
	// Sqr distance between vec1 and vec2
	static Type DistanceSqr(const Vector<Type, Dim> & vec1, const Vector<Type, Dim> & vec2);
	// Dot product of vec1 and vec2
	static Type Dot(const Vector<Type, Dim> & vec1, const  Vector<Type, Dim> & vec2);
	static void Normalize(Vector<Type, Dim> & vec);

	inline void Copy(Vector<Type, Dim> & out);
	inline bool ApproximateCompare(const Vector<Type, Dim> & vec);
	// Length of this vector
	inline Type Length();
	// Sqr length of this vector
	inline Type LengthSqr();
	// Manhattan norm
	inline Type ManhattanNorm();
	// Distance between this and vec
	inline Type Distance(const Vector<Type, Dim> & vec);
	// Sqr distance between this and vec
	inline Type DistanceSqr(const Vector<Type, Dim> & vec);
	// Dot product of this and vec
	inline Type Dot(const Vector<Type, Dim> & vec);
	inline void Normalize();
	inline Vector<Type, Dim> GetNormalized() const;

	static Vector<float, 3>Cross(const Vector<Type, Dim> & in1, const  Vector<Type, Dim> & in2);
	inline Vector<float, 3>Cross(const Vector<Type, Dim> & in);

	inline const Type *ToArray() const;
};

/*
==========
Vectors
==========
*/
template <class Type, int Dim>
inline Vector<Type, Dim>::Vector()
{
	/*for (int i = 0; i < Dim; i++)
	{
		vector[i] = (Type)0;
	}*/
}

template<class Type, int Dim>
inline Vector<Type, Dim>::Vector(const Type x)
{
	for (int i = 0; i < Dim; i++)
	{
		vector[i] = x;
	}
}

template<class Type, int Dim>
inline Vector<Type, Dim>::Vector(const Type x, const Type y)
{
	if (Dim >= 2)
	{
		vector[0] = x;
		vector[1] = y;
	}
}

template<class Type, int Dim>
inline Vector<Type, Dim>::Vector(const Type x, const Type y, const Type z)
{
	if (Dim >= 3)
	{
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
	}
}

template<class Type, int Dim>
inline Vector<Type, Dim>::Vector(const Type x, const Type y, const Type z, const Type w)
{
	if (Dim >= 4)
	{
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		vector[3] = w;
	}
}

template <class Type, int Dim>
inline Type &Vector<Type, Dim>::operator[](int index)
{
	ASSERT(index < Dim);

	return vector[index];
}

template <class Type, int Dim>
inline const Type Vector<Type, Dim>::operator[](int index) const
{
	ASSERT(index < Dim);

	return vector[index];
}

template <class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::operator+(const Vector<Type, Dim>in) const
{
	Vector<Type, Dim>result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vector[i] + in.vector[i];
	}

	return result;
}

template <class Type, int Dim>
inline Vector<Type, Dim> &Vector<Type, Dim>::operator+=(const Vector<Type, Dim>in)
{
	for (int i = 0; i < Dim; i++)
	{
		vector[i] += in.vector[i];
	}

	return *this;
}

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::operator-() const
{
	return (*this) * (Type)(-1);
}

template <class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::operator-(const Vector<Type, Dim>in) const
{
	Vector<Type, Dim>result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vector[i] - in.vector[i];
	}

	return result;
}

template <class Type, int Dim>
inline Vector<Type, Dim> &Vector<Type, Dim>::operator-=(const Vector<Type, Dim>in)
{
	for (int i = 0; i < Dim; i++)
	{
		vector[i] -= in.vector[i];
	}

	return *this;
}

template <class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::operator*(const Type scale) const
{
	Vector<Type, Dim>result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vector[i] * scale;
	}

	return result;
}

template <class Type, int Dim>
inline Vector<Type, Dim> &Vector<Type, Dim>::operator*=(const Type scale)
{
	for (int i = 0; i < Dim; i++)
	{
		vector[i] *= scale;
	}

	return *this;
}

template <class Type, int Dim>
inline bool Vector<Type, Dim>::operator==(const Vector<Type, Dim>vec)
{
	for (int i = 0; i < Dim; i++)
	{
		if (vector[i] != vec[i])
		{
			return false;
		}
	}

	return true;
}

template<class Type, int Dim>
inline bool Vector<Type, Dim>::operator==(const Vector<Type, Dim> vec) const
{
	for (int i = 0; i < Dim; i++)
	{
		if (vector[i] != vec[i])
		{
			return false;
		}
	}

	return true;
}

template <class Type, int Dim>
inline bool Vector<Type, Dim>::operator!=(const Vector<Type, Dim>vec)
{
	for (int i = 0; i < Dim; i++)
	{
		if (vector[i] == vec[i])
		{
			return false;
		}
	}

	return true;
}

template <class Type, int Dim>
inline void Vector<Type, Dim>::Copy(const Vector<Type, Dim>&in, Vector<Type, Dim>&out)
{
	for (int i = 0; i < Dim; i++)
	{
		out[i] = in[i];
	}
}

template <class Type, int Dim>
inline bool Vector<Type, Dim>::ApproximateCompare(const Vector<Type, Dim>&vec1, const Vector<Type, Dim>&vec2)
{
	for (int i = 0; i < Dim; i++)
	{
		if (ABS(vec1[i] - vec2[i]) > EPSILONEQUAL)
		{
			return false;
		}
	}

	return true;
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Length(const Vector<Type, Dim>&vec)
{
	Type length = Vector<Type, Dim>::LengthSqr(vec);
	length = (Type)Sqrt(length);

	return length;
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::LengthSqr(const Vector<Type, Dim>&vec)
{
	return Dot(vec, vec);
}

template<class Type, int Dim>
inline Type Vector<Type, Dim>::ManhattanNorm(const Vector<Type, Dim>& vec)
{
	Type mn = (Type)0;

	for (int i = 0; i < Dim; i++)
	{
		mn += Abs(vec[i]);
	}

	return mn;
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Distance(const Vector<Type, Dim>& vec1, const Vector<Type, Dim>& vec2)
{
	return Length(vec1 - vec2);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::DistanceSqr(const Vector<Type, Dim>& vec1, const Vector<Type, Dim>& vec2)
{
	return LengthSqr(vec1 - vec2);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Dot(const Vector<Type, Dim>&vec1, const Vector<Type, Dim>&vec2)
{
	Type result = (Type)0;

	for (int i = 0; i < Dim; i++)
	{
		result += vec1[i] * vec2[i];
	}

	return result;
}

template <class Type, int Dim>
inline void Vector<Type, Dim>::Normalize(Vector<Type, Dim>&vec)
{
	Type length = Vector<Type, Dim>::Length(vec);

	ASSERT(length != 0);

	// inverted length
	length = (Type)1 / length;

	for (int i = 0; i < Dim; i++)
	{
		vec[i] *= length;
	}
}

template <class Type, int Dim>
inline void Vector<Type, Dim>::Copy(Vector<Type, Dim>&out)
{
	Copy(*this, out);
}

template <class Type, int Dim>
inline bool Vector<Type, Dim>::ApproximateCompare(const Vector<Type, Dim>&vec)
{
	return ApproximateCompare(*this, vec);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Length()
{
	return Length(*this);
}

template <class Type, int Dim>
Type Vector<Type, Dim>::LengthSqr()
{
	return LengthSqr(*this);
}

template<class Type, int Dim>
inline Type Vector<Type, Dim>::ManhattanNorm()
{
	return ManhattanNorm(*this);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Distance(const Vector<Type, Dim>& vec)
{
	return Distance(*this, vec);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::DistanceSqr(const Vector<Type, Dim>& vec)
{
	return DistanceSqr(*this, vec);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Dot(const Vector<Type, Dim>&vec)
{
	return Dot(*this, vec);
}

template <class Type, int Dim>
inline void Vector<Type, Dim>::Normalize()
{
	Normalize(*this);
}

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::GetNormalized() const
{
	Vector<Type, Dim> result(*this);
	result.Normalize();
	return result;
}

#pragma region vector3
/*const Vector<Type, Dim>Vector::Zero = Vector(0.0f, 0.0f, 0.0f);
const Vector<Type, Dim>Vector::Right = Vector(1.0f, 0.0f, 0.0f);
const Vector<Type, Dim>Vector::Up = Vector(0.0f, 1.0f, 0.0f);
const Vector<Type, Dim>Vector::Forward = Vector(0.0f, 0.0f, 1.0f);*/

template <>
inline Vector<float, 3> Vector<float, 3>::Cross(const Vector<float, 3>&in1, const Vector<float, 3>&in2)
{
	Vector<float, 3> out = Vector<float, 3>(
		in1[1] * in2[2] - in1[2] * in2[1],
		in1[2] * in2[0] - in1[0] * in2[2],
		in1[0] * in2[1] - in1[1] * in2[0] );

	return out;
}

template <>
inline Vector<float, 3> Vector<float, 3>::Cross(const Vector<float, 3>&in)
{
	return Cross(*this, in);
}
#pragma endregion

template<class Type, int Dim>
inline const Type *Vector<Type, Dim>::ToArray() const
{
	return &vector[0];
}