#pragma once

// Null terminated string allocated in a heap
class String
{
private:
	char *string;
	UINT length;	// string length without '\0'
					// memory allocated for string is (length+1)

private:
	// Allocates new string
	static char				*AllocateString(const char* orig);

public:
	// Creates empty string ("")
	inline String();
	// Creates copy of orig
	inline String(const String &orig);
	// Creates copy from orig
	String(const char* orig);
	// Destructor
	~String();

	inline char				&operator[](UINT i);
	inline char				operator[](UINT i) const;

	inline bool				operator==(const String &b) const;
	bool					operator==(const char *b) const;
	inline bool				operator!=(const String &b) const;
	inline bool				operator!=(const char *b) const;

	inline String			&operator=(const String &b);
	String					&operator=(const char *b);

	// Overloaded cast to const char*
	inline operator const char * () const;

	// Concatenate
	inline String			operator+(const String &b) const;
	// Concatenate
	String					operator+(const char *b) const;
	// Concatenate
	inline String			&operator+=(const String &b);
	// Concatenate
	String					&operator+=(const char *b);

	// Returns length of this string
	inline UINT				Length() const;

	// Makes string empty
	inline void				Clear();
	// Splits this string by pos
	// Char with index "pos" will be in "b"
	inline void				Split(UINT pos, String &a, String &b) const;
	// Removes chars from right
	inline void				Remove(UINT fromRight);
	// Removes chars
	void					Remove(UINT fromLeft, UINT fromRight);

	static bool				ToBool(const char *str);
	static int				ToInt(const char *str);
	static float			ToFloat(const char *str);
	static Vector3			ToVector3(const char *str);
	static Quaternion		ToQuaternion(const char *str);

	inline bool				ToBool() const;
	inline int				ToInt() const;
	inline float			ToFloat() const;
	Vector3					ToVector3() const;
	Quaternion				ToQuaternion() const;

	inline const char		*GetCharPtr() const;

	// Hash function for a string
	static UINT				StringHash(String toHash);

	// Use this function ONLY if it was allocated manually
	// (through malloc or other, there is garbage in the memory to set in)
	void					Init(const char *str);
	// Free string memory
	void					Delete();
};

inline String::String() : String("")
{ }

inline String::String(const String &orig) : String(orig.string)
{ }

inline char &String::operator[](UINT i)
{
	ASSERT(i <= length); // == to allow to read '\0'
	return string[i];
}

inline char String::operator[](UINT i) const
{
	ASSERT(i <= length); // == to allow to read '\0'
	return string[i];
}

inline bool String::operator==(const String &b) const
{
	return (*this) == b.string;
}

inline bool String::operator!=(const String &b) const
{
	return !(*this == b);
}

inline bool String::operator!=(const char * b) const
{
	return !(*this == b);
}

inline String &String::operator=(const String &b)
{
	*this = b.string;
	return *this;
}

inline String::operator const char*() const
{
	return string;
}

inline String String::operator+(const String &b) const
{
	return (*this + b.string);
}

inline String &String::operator+=(const String &b)
{
	*this += b.string;
	return *this;
}

inline UINT String::Length() const
{
	return length;
}

inline void String::Clear()
{
	*this = "";
}

inline void String::Split(UINT pos, String &a, String &b) const
{
	ASSERT(pos < length);

	// create copies
	a = string;
	b = string;

	a.Remove(0, length - pos);
	b.Remove(pos, 0);
}

inline void String::Remove(UINT fromRight)
{
	Remove(0, fromRight);
}

inline bool String::ToBool() const
{
	return ToBool(string);
}

inline int String::ToInt() const
{
	return ToInt(string);
}

inline float String::ToFloat() const
{
	return ToFloat(string);
}

inline const char * String::GetCharPtr() const
{
	return string;
}

inline UINT String::StringHash(String toHash)
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