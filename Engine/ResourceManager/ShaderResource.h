#pragma once

class ShaderResource
{
	friend class ResourceManager;

private:
	// Vertex shader code
	String vertexCode;
	// Fragment shader code
	String fragmentCode;

	// Paths to sources
	String vertexPath;
	String fragmentPath;

public:
	// Empty destructor, strings will automatically destroyed
	~ShaderResource();

	// Get vertex shader code
	const String &GetVertexCode() const;
	// Get fragment shader code
	const String &GetFragmentCode() const;

	const String &GetVertexPath() const;
	const String &GetFragmentPath() const;
};

inline ShaderResource::~ShaderResource()
{ }

inline const String &ShaderResource::GetVertexCode() const
{
	return vertexCode;
}

inline const String &ShaderResource::GetFragmentCode() const
{
	return fragmentCode;
}

inline const String & ShaderResource::GetVertexPath() const
{
	return vertexPath;
}

inline const String & ShaderResource::GetFragmentPath() const
{
	return fragmentPath;
}
