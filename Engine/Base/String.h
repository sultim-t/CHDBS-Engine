#pragma once

// Null terminated string, memory is allocated in a heap
class String
{
private:
	char *string;
	UINT length;

private:
	// Allocates new string
	static char *AllocateString(const char* orig);

public:
	// Creates empty string ("")
	String();
	// Creates copy from orig
	String(const char* orig);
	// Creates copy of orig
	String(const String &orig);
	// Destructor
	~String();

	char operator[](UINT i) const;

	bool operator==(const String &b) const;
	bool operator==(const char *b) const;
	bool operator!=(const String &b) const;
	bool operator!=(const char *b) const;

	String &operator=(const String &b);
	String &operator=(const char *b);

	// Overloaded cast to const char*
	operator const char * () const;

	// Concatenate
	String operator+(const String &b) const;
	// Concatenate
	String operator+(const char *b) const;
	// Concatenate
	String &operator+=(const String &b);
	// Concatenate
	String &operator+=(const char *b);

	// Returns length of this string
	UINT Length() const;

	// Makes string empty
	void Clear();
	// Splits this string by pos
	// Char with index "pos" will be in "b"
	void Split(UINT pos, String &a, String &b) const;
	// Removes chars after pos
	void Remove(UINT pos);
	// Removes chars after pos
	void Remove(UINT fromLeft, UINT fromRight);

	INT ToInt() const;
	FLOAT ToFloat() const;

	const char *GetCharPtr() const;

	// Hash function for a string
	// Can be used in hash table
	static UINT StringHash(String toHash);
};