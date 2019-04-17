#include <Engine/Memory/Memory.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <string>
#include "String.h"

String::String(const char * orig) :
	string(AllocateString(orig)),
	length(strlen(string)) { }

String::~String()
{
	SystemAllocator::Free(string);
}

char * String::AllocateString(const char * orig)
{
	ASSERT(orig != nullptr);
	// length of string
	UINT size = strlen(orig);

	// allocate memory for copy, +1 for '\0'
	char *copy = (char*)SystemAllocator::Allocate(size + 1);
	// copy, +1 for '\0'
	memcpy(copy, orig, size + 1);

	return copy;
}

void String::Init(const char *str)
{
	string = AllocateString(str);
	length = strlen(string);
}

inline void String::Delete()
{
	SystemAllocator::Free(string);
}

String &String::operator=(const char * b)
{
	ASSERT(b != nullptr);

	// allocate copy
	char *copy = AllocateString(b);

	if (string != NULL)
	{
		// then deallocate current
		SystemAllocator::Free(string);
	}

	// and reassign
	string = copy;

	return *this;
}

String String::operator+(const char * b) const
{
	ASSERT(b != nullptr);

	String copy;

	UINT curSize = strlen(string);
	UINT bSize = strlen(b);

	UINT size = curSize + bSize;

	// +1 for '\0'
	copy.string = (char*)SystemAllocator::Allocate(size + 1);

	// copy from this to copy without '\0'
	memcpy(copy.string, string, curSize);
	// copy from b to copy with '\0'
	// starting after char with index curSize
	memcpy(copy.string + curSize, b, bSize + 1);

	copy.length = size;

	return copy;
}

String &String::operator+=(const char * b)
{
	ASSERT(b != nullptr);

	UINT curSize = strlen(string);
	UINT bSize = strlen(b);

	UINT size = curSize + bSize;

	// +1 for '\0'
	string = (char*)SystemAllocator::Reallocate((void*)string, curSize + 1, size + 1);

	// copy from b to string
	// strcat(string, b);
	strncat_s(string, size, b, bSize);

	length = size;

	return *this;
}

void String::Remove(UINT fromLeft, UINT fromRight)
{
	if (length <= fromRight || length <= fromLeft)
	{
		return;
	}

	if (fromRight > 0)
	{
		UINT newLength = length - fromRight;

		// set new end
		string[newLength] = '\0';

		// +1 for '\0'
		string = (char*)SystemAllocator::Reallocate(string, length + 1, newLength + 1);

		// reassign length
		length = newLength;
	}
	
	if (fromLeft > 0)
	{
		UINT newLength = length - fromLeft;

		// set new start
		// memmove supports overlapping
		// +1 for copying '\0'
		memmove(string, &string[newLength], newLength + 1);

		// +1 for '\0'
		string = (char*)SystemAllocator::Reallocate(string, length + 1, newLength + 1);

		// reassign length
		length = newLength;
	}
}

Vector3 String::ToVector3(const char *str)
{
	const int Dim = 3;

	Vector3 result;
	UINT index = 0;

	// create copy
	char *temp = AllocateString(str);

	// pointer to the beginning of float to parse
	const char *ptr = temp;

	UINT length = strlen(str);

	// <= to check last symbol
	for (UINT i = 0; i <= length; i++)
	{
		if (str[i] == ' ' || str[i] == '\0')
		{
			ASSERT(index < Dim);

			temp[i] = '\0';

			// convert to float till '\0'
			result[index] = (float)atof(ptr);

			// update ptr
			ptr = temp + i + 1;
			index++;
		}
	}

	// delete copy
	SystemAllocator::Free(temp);

	// convert unparsed to zero
	for (UINT i = index; i < Dim; i++)
	{
		result[i] = 0;
	}

	return result;
}

Quaternion String::ToQuaternion(const char * str)
{
	const int Dim = 4;

	Quaternion result;
	UINT index = 0;

	// create copy
	char *temp = AllocateString(str);

	// pointer to the beginning of float to parse
	const char *ptr = temp;

	UINT length = strlen(str);

	// <= to check last symbol
	for (UINT i = 0; i <= length; i++)
	{
		if (str[i] == ' ' || str[i] == '\0')
		{
			ASSERT(index < Dim);

			temp[i] = '\0';

			// convert to float till '\0'
			result[index] = (float)atof(ptr);

			// update ptr
			ptr = temp + i + 1;
			index++;
		}
	}

	// delete copy
	SystemAllocator::Free(temp);

	// convert unparsed to zero
	for (UINT i = index; i < Dim; i++)
	{
		result[i] = 0;
	}

	return result;
}

Color4 String::ToColor4(const char * str)
{
	const int Dim = 4;

	Color4 result;
	UINT index = 0;

	// create copy
	char *temp = AllocateString(str);

	// pointer to the beginning of float to parse
	const char *ptr = temp;

	UINT length = strlen(str);

	// <= to check last symbol
	for (UINT i = 0; i <= length; i++)
	{
		if (str[i] == ' ' || str[i] == '\0')
		{
			ASSERT(index < Dim);

			temp[i] = '\0';

			// convert to float till '\0'
			result[index] = (UBYTE)atoi(ptr);

			// update ptr
			ptr = temp + i + 1;
			index++;
		}
	}

	// delete copy
	SystemAllocator::Free(temp);

	// convert unparsed to zero
	for (UINT i = index; i < Dim; i++)
	{
		if (i == 3)
		{
			// if alpha
			result[i] = 255;
		}
		else
		{
			result[i] = 0;
		}
	}

	return result;
}

bool String::operator==(const char * b) const
{
	return strcmp(string, b) == 0;
}

bool String::ToBool(const char *str)
{
	return ToInt(str) != 0;
}

int String::ToInt(const char *str)
{
	return atoi(str);
}

float String::ToFloat(const char *str)
{
	return (float)atof(str);
}

Vector3 String::ToVector3() const
{
	return ToVector3(string);
}

Quaternion String::ToQuaternion() const
{
	return ToQuaternion(string);
}

Color4 String::ToColor4() const
{
	return ToColor4(string);
}
