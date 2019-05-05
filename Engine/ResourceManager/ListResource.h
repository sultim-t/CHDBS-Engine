#pragma once
#include <Engine/DataStructures/StaticArray.h>

class ListResource
{
	friend class ResourceManager;

private:
	// Path to this resource
	String path;
	// List of lines
	StaticArray<String> lines;

public:
	// Default constructor
	ListResource(const char *path);
	// Destructor
	~ListResource();

	void Init(int linesCount);
	
	// Get all paths to entity files in this scene
	const StaticArray<String> &GetLines() const;
	// Get path to this scene resource
	const String &GetPath() const;
};

inline ListResource::ListResource(const char * path)
	: path(path) { }

inline ListResource::~ListResource()
{
	for (UINT i = 0; i < lines.GetSize(); i++)
	{
		lines[i].Delete();
	}
}

inline void ListResource::Init(int linesCount)
{
	lines.Init(linesCount);
}

inline const StaticArray<String> &ListResource::GetLines() const
{
	return lines;
}

inline const String &ListResource::GetPath() const
{
	return path;
}