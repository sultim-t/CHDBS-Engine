#pragma once

class ShaderResource
{
	friend class ResourceManager;

private:
	// Vertex shader code
	String vertexCode;
	// Fragment shader code
	String fragmentCode;

public:
	// Empty destructor, strings will automatically destroyed
	~ShaderResource();

	// Get vertex shader code
	const String &GetVertexCode() const;
	// Get fragment shader code
	const String &GetFragmentCode() const;
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
