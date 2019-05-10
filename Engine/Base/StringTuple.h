#pragma once

class StringTuple
{
private:
	String left;
	String right;

public:
	// Default constructor
	StringTuple() { }
	
	StringTuple(const char *left, const char *right)
	{
		this->left = left;
		this->right = right;
	}

	// Manually delete strings
	// Use this function to delete strings in arrays, which contatains shared_ptr or same (they call destructor only on first element)
	void Delete()
	{
		left.Delete();
		right.Delete();
	}

	StringTuple &operator=(const StringTuple &source)
	{
		this->left = source.left;
		this->right = source.right;
	}

	// Use this function if target memory has garbage
	void Init(const char *left, const char *right)
	{
		this->left = left;
		this->right = right;
	}

	// Use this function if target memory has garbage
	void Init(const StringTuple &source)
	{
		this->left = source.left;
		this->right = source.right;
	}

	const String &Left() const
	{
		return left;
	}

	const String &Right() const
	{
		return right;
	}
};
