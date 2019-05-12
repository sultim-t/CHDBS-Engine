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
		Set(left, right);
	}

	StringTuple(const StringTuple &source)
	{
		Set(source.left, source.right);
	}

	StringTuple &operator=(const StringTuple &source)
	{
		Set(source.left, source.right);
		return *this;
	}

	bool operator==(const StringTuple &source)
	{
		return left == source.left && right == source.right;
	}

	const String &Left() const
	{
		return left;
	}

	const String &Right() const
	{
		return right;
	}

	void Set(const char *left, const char *right)
	{
		SetLeft(left);
		SetRight(right);
	}

	void SetLeft(const char *left)
	{
		this->left = left;
	}

	void SetRight(const char *right)
	{
		this->right = right;
	}

	void Delete()
	{
		left.Delete();
		right.Delete();
	}
};
