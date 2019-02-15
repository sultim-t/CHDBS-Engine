#include "String.h"
#include <Engine/Memory/Memory.h>
#include <string>

String::String() : String("")
{ }

String::String(const String & orig) : String(orig.string)
{ }

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

char String::operator[](UINT i) const
{
	ASSERT(i < length);
	return string[i];
}

bool String::operator==(const String & b) const
{
	// if 0 then equal
	return strcmp(string, b.string) == 0;
}

bool String::operator==(const char * b) const
{
	return strcmp(string, b) == 0;
}

bool String::operator!=(const String & b) const
{
	return !(*this == b);
}

bool String::operator!=(const char * b) const
{
	return !(*this == b);
}

String & String::operator=(const String & b)
{
	*this = b.string;
	return *this;
}

String & String::operator=(const char * b)
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

String::operator const char*() const
{
	return string;
}

String String::operator+(const String & b) const
{
	return (*this + b.string);
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

String & String::operator+=(const String & b)
{
	*this += b.string;
	return *this;
}

String & String::operator+=(const char * b)
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

UINT String::Length() const
{
	return length;
}

void String::Clear()
{
	*this = "";
}

void String::Split(UINT pos, String & a, String & b) const
{
	ASSERT(pos < length);

	// create copies
	a = string;
	b = string;

	a.Remove(0, length - pos);
	b.Remove(pos, 0);
}

void String::Remove(UINT pos)
{
	Remove(0, pos);
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

int String::ToInt() const
{
	return atoi(string);
}

float String::ToFloat() const
{
	return (float)atof(string);
}

const char * String::GetCharPtr() const
{
	return string;
}

UINT String::StringHash(String toHash)
{
	// djb2
	// modified: using UINT

	unsigned char *str = (unsigned char*)toHash.string;

	UINT hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}
