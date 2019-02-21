#include <Engine/Memory/Memory.h>
#include <Engine/Math/Vector.h>
#include <string>

String::String(const char * orig)
{
	string = AllocateString(orig);
	length = strlen(string);
}

String::~String()
{
	SYSALLOCATOR.Free(string);
}

char * String::AllocateString(const char * orig)
{
	ASSERT(orig != nullptr);
	// length of string
	UINT size = strlen(orig);

	// allocate memory for copy, +1 for '\0'
	char *copy = (char*)SYSALLOCATOR.Allocate(size + 1);
	// copy, +1 for '\0'
	memcpy(copy, orig, size + 1);

	return copy;
}

bool String::operator==(const char * b) const
{
	return strcmp(string, b) == 0;
}

String &String::operator=(const char * b)
{
	ASSERT(b != nullptr);

	// allocate copy
	char *copy = AllocateString(b);

	if (string != NULL)
	{
		// then deallocate current
		SYSALLOCATOR.Free(string);
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
	copy.string = (char*)SYSALLOCATOR.Allocate(size + 1);

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
	string = (char*)SYSALLOCATOR.Reallocate((void*)string, curSize + 1, size + 1);

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
		string = (char*)SYSALLOCATOR.Reallocate(string, length + 1, newLength + 1);

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
		string = (char*)SYSALLOCATOR.Reallocate(string, length + 1, newLength + 1);

		// reassign length
		length = newLength;
	}
}

bool String::ToBool() const
{
	return (bool)ToInt();
}

int String::ToInt() const
{
	return atoi(string);
}

float String::ToFloat() const
{
	return (float)atof(string);
}

Vector3 String::ToVector3() const
{
	Vector3 result;
	UINT index = 0;

	String temp(string);

	// pointer to the beginning of float to parse
	const char *ptr = temp.GetCharPtr();
	
	// <= to check last symbol
	for (UINT i = 0; i <= length; i++)
	{
		if (string[i] == ' ' || string[i] == '\0')
		{
			ASSERT(index < 3);

			temp[i] = '\0';

			// convert to float till '\0'
			result[index] = (float)atof(ptr);

			// update ptr
			ptr = temp + i + 1;
			index++;
		}
	}

	// convert unparsed to zero
	for (UINT i = index; i < 3; i++)
	{
		result[i] = 0;
	}

	return result;
}
