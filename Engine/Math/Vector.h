#pragma once

#include "Math.h"

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
	// Init from another vector, 0 is setted to uninitialized component
	template <int ADim>
	inline Vector(const Vector<Type, ADim> &vec);
	// Init from another vector, "t" is setted to uninitialized component
	template <int ADim>
	inline Vector(const Vector<Type, ADim> &vec, const Type t);

	inline Type &operator[](int index);
	inline const Type operator[](int index) const;
	inline Vector<Type, Dim>&operator=(const Vector<Type, Dim> &in);
	
	inline Vector<Type, Dim>operator+(const Vector<Type, Dim> &in) const;
	inline Vector<Type, Dim>&operator+=(const Vector<Type, Dim> &in);
	inline Vector<Type, Dim>operator-() const;
	inline Vector<Type, Dim>operator-(const Vector<Type, Dim> &in) const;
	inline Vector<Type, Dim>&operator-=(const Vector<Type, Dim> &in);
	inline Vector<Type, Dim>operator*(const Type scale) const;
	inline Vector<Type, Dim>&operator*=(const Type scale);
	inline Vector<Type, Dim>operator/(const Type scale) const;
	inline Vector<Type, Dim>&operator/=(const Type scale);
	
	inline bool operator==(const Vector<Type, Dim> &vec);
	inline bool operator==(const Vector<Type, Dim> &vec) const;
	inline bool operator!=(const Vector<Type, Dim> &vec);
	inline bool operator!=(const Vector<Type, Dim> &vec) const;

	static void Copy(const Vector<Type, Dim> &in, Vector<Type, Dim> &out);
	static bool ApproximateCompare(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2);
	// Length of vec
	static Type Length(const Vector<Type, Dim> &vec);
	// Sqr length vec
	static Type LengthSqr(const Vector<Type, Dim> &vec);
	// Manhattan norm
	static Type ManhattanNorm(const Vector<Type, Dim> &vec);
	// Distance between vec1 and vec2
	static Type Distance(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2);
	// Sqr distance between vec1 and vec2
	static Type DistanceSqr(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2);
	// Dot product of vec1 and vec2
	static Type Dot(const Vector<Type, Dim> &vec1, const  Vector<Type, Dim> &vec2);
	static void Normalize(Vector<Type, Dim> &vec);
	// Multiplies vectors component-wise
	static Vector<Type, Dim> Scale(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2);

	inline void Copy(Vector<Type, Dim> &out) const;
	inline bool ApproximateCompare(const Vector<Type, Dim> &vec) const;
	// Length of this vector
	inline Type Length() const;
	// Sqr length of this vector
	inline Type LengthSqr() const;
	// Manhattan norm
	inline Type ManhattanNorm() const;
	// Distance between this and vec
	inline Type Distance(const Vector<Type, Dim> &vec) const;
	// Sqr distance between this and vec
	inline Type DistanceSqr(const Vector<Type, Dim> &vec) const;
	// Dot product of this and vec
	inline Type Dot(const Vector<Type, Dim> &vec) const;
	inline void Normalize();
	inline Vector<Type, Dim> GetNormalized() const;
	// Multiply by vector component-wise
	inline Vector<Type, Dim> Scale(const Vector<Type, Dim> &vec) const;

	// Reflect vector along normal
	// Note: normal can be unnormalized
	static Vector<Type, Dim> Reflect(const Vector<Type, Dim> &vec, const Vector<Type, Dim> &normal);

	static Vector<Type, Dim> Lerp(const Vector<Type, Dim> &start, const Vector<Type, Dim> &end, const float t);

	static Vector<float, 3>Cross(const Vector<Type, Dim> &in1, const  Vector<Type, Dim> &in2);
	inline Vector<float, 3>Cross(const Vector<Type, Dim> &in) const;

	inline const Type *ToArray() const;
};

// gloabal overload; to use (scale*Vector)
//template<class Type, int Dim>
//inline Vector<Type, Dim> operator*(const Type scale, const Vector<Type, Dim>& vec) const;

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
	ASSERT(Dim >= 2);

	vector[0] = x;
	vector[1] = y;
}

template<class Type, int Dim>
inline Vector<Type, Dim>::Vector(const Type x, const Type y, const Type z)
{
	ASSERT(Dim >= 3);

	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}

template<class Type, int Dim>
inline Vector<Type, Dim>::Vector(const Type x, const Type y, const Type z, const Type w)
{
	ASSERT(Dim >= 4);

	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;
}

template<class Type, int Dim>
template<int ADim>
inline Vector<Type, Dim>::Vector(const Vector<Type, ADim> &vec)
{
	int i = 0;

	if (ADim <= Dim)
	{
		for (; i < ADim; i++)
		{
			vector[i] = vec[i];
		}
		for (; i < Dim; i++)
		{
			vector[i] = (Type)0;
		}
	}
	else
	{
		for (; i < Dim; i++)
		{
			vector[i] = vec[i];
		}
	}
}

template<class Type, int Dim>
template<int ADim>
inline Vector<Type, Dim>::Vector(const Vector<Type, ADim>& vec, const Type t)
{
	int a = Dim <= ADim ? Dim : ADim;
	int b = a == Dim ? ADim : Dim;

	for (int i = 0; i < a; i++)
	{
		vector[i] = vec[i];
	}
	for (int i = a; i < b; i++)
	{
		vector[i] = t;
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

template<class Type, int Dim>
inline Vector<Type, Dim>& Vector<Type, Dim>::operator=(const Vector<Type, Dim>& in)
{
	Copy(in, *this);
	return *this;
}

template <class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::operator+(const Vector<Type, Dim> &in) const
{
	Vector<Type, Dim>result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vector[i] + in.vector[i];
	}

	return result;
}

template <class Type, int Dim>
inline Vector<Type, Dim> &Vector<Type, Dim>::operator+=(const Vector<Type, Dim> &in)
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
inline Vector<Type, Dim> Vector<Type, Dim>::operator-(const Vector<Type, Dim> &in) const
{
	Vector<Type, Dim>result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vector[i] - in.vector[i];
	}

	return result;
}

template <class Type, int Dim>
inline Vector<Type, Dim> &Vector<Type, Dim>::operator-=(const Vector<Type, Dim> &in)
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

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::operator/(const Type scale) const
{
	Vector<Type, Dim> result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vector[i] / scale;
	}

	return result;
}

template<class Type, int Dim>
inline Vector<Type, Dim>& Vector<Type, Dim>::operator/=(const Type scale)
{
	for (int i = 0; i < Dim; i++)
	{
		vector[i] /= scale;
	}

	return *this;
}

template <class Type, int Dim>
inline bool Vector<Type, Dim>::operator==(const Vector<Type, Dim> &vec)
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
inline bool Vector<Type, Dim>::operator==(const Vector<Type, Dim> &vec) const
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
inline bool Vector<Type, Dim>::operator!=(const Vector<Type, Dim> &vec)
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

template<class Type, int Dim>
inline bool Vector<Type, Dim>::operator!=(const Vector<Type, Dim>& vec) const
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
inline Type Vector<Type, Dim>::ManhattanNorm(const Vector<Type, Dim> &vec)
{
	Type mn = (Type)0;

	for (int i = 0; i < Dim; i++)
	{
		mn += Abs(vec[i]);
	}

	return mn;
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Distance(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2)
{
	return Length(vec1 - vec2);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::DistanceSqr(const Vector<Type, Dim> &vec1, const Vector<Type, Dim> &vec2)
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

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::Scale(const Vector<Type, Dim>& vec1, const Vector<Type, Dim>& vec2)
{
	Vector<Type, Dim> result;

	for (int i = 0; i < Dim; i++)
	{
		result[i] = vec1[i] * vec2[i];
	}
	
	return result;
}

template <class Type, int Dim>
inline void Vector<Type, Dim>::Copy(Vector<Type, Dim>&out) const
{
	Copy(*this, out);
}

template <class Type, int Dim>
inline bool Vector<Type, Dim>::ApproximateCompare(const Vector<Type, Dim>&vec) const
{
	return ApproximateCompare(*this, vec);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Length() const
{
	return Length(*this);
}

template <class Type, int Dim>
Type Vector<Type, Dim>::LengthSqr() const
{
	return LengthSqr(*this);
}

template<class Type, int Dim>
inline Type Vector<Type, Dim>::ManhattanNorm() const
{
	return ManhattanNorm(*this);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Distance(const Vector<Type, Dim> &vec) const
{
	return Distance(*this, vec);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::DistanceSqr(const Vector<Type, Dim> &vec) const
{
	return DistanceSqr(*this, vec);
}

template <class Type, int Dim>
inline Type Vector<Type, Dim>::Dot(const Vector<Type, Dim>&vec) const
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

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::Scale(const Vector<Type, Dim>& vec) const
{
	*this = Scale(*this, vec);
}

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::Reflect(const Vector<Type, Dim>& vec, const Vector<Type, Dim>& normal)
{
	ASSERT(normal.LengthSqr() != 0.0f);
	return vec - normal * 2 * Dot(vec, normal) / normal.LengthSqr();
}

template<class Type, int Dim>
inline Vector<Type, Dim> Vector<Type, Dim>::Lerp(const Vector<Type, Dim>& start, const Vector<Type, Dim>& end, const float t)
{
	return start * ((Type)1 - t) + end * t;
}

#pragma region vector3
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
inline Vector<float, 3> Vector<float, 3>::Cross(const Vector<float, 3>&in) const
{
	return Cross(*this, in);
}
#pragma endregion

template<class Type, int Dim>
inline const Type *Vector<Type, Dim>::ToArray() const
{
	return &vector[0];
}

// gloabal overload; to use (scale*Vector)
/*template<class Type, int Dim>
inline Vector<Type, Dim> operator*(const Type scale, const Vector<Type, Dim>& vec) const
{
	return vec * scale;
}*/