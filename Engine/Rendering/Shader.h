#pragma once
#include <Engine/ResourceManager/ShaderResource.h>

// Represents compiled shader program
class Shader
{
	friend class Material;

private:
	// Unique ID of this shader
	UINT		shaderId;
	// Unique shader name
	String		name;

	// Is this shader compiled?
	bool		isCompiled;

	// Program ID
	UINT		graphicsProgramId;
	UINT		vertId;
	UINT		fragId;

	// Reference to shader resource
	// which holds only text files
	const ShaderResource *resource;

private:
	// Load shader from strings
	void Compile(const char *vertexCode, const char *fragmentCode);

public:
	// Reads and builds the shader
	void Load(const char *vertexPath, const char *fragmentPath);

	// Get uniform location
	int GetUniformLocation(const char *name) const;

	// Uniform functions
	void SetBool(const char *name, bool value) const;
	void SetInt(const char *name, int value) const;
	void SetFloat(const char *name, float value) const;

	void SetVec2(const char *name, const Vector2 &vec) const;
	void SetVec2(const char *name, float x, float y) const;
	void SetVec3(const char *name, const Vector3 &vec) const;
	void SetVec3(const char *name, float x, float y, float z) const;
	void SetVec4(const char *name, const Vector4 &vec) const;
	void SetVec4(const char *name, float x, float y, float z, float w) const;

	void SetMat2(const char *name, const Matrix2 &mat) const;
	void SetMat3(const char *name, const Matrix3 &mat) const;
	void SetMat4(const char *name, const Matrix4 &mat) const;

	void SetInt(int location, int value) const;
	void SetFloat(int location, float value) const;
	void SetVec3(int location, const Vector3 &vec) const;
	void SetVec4(int location, const Vector4 &vec) const;
	void SetMat4(int location, const Matrix4 &mat) const;

public:
	// Empty constructor
	Shader(const char *name);
	// Unregisters shader
	~Shader();

	// Register shader in rendering system
	//virtual void Init();

	// Use/activate the shader
	void Use() const;
	// Deactivate
	void Stop() const;

	int GetProgramID() const;
	const String &GetName() const;

public:
	// Static function to register shader
	static const Shader *RegisterShader(const char *shaderName, const char *vertPath, const char *fragPath);
	// Static function to get shader by name
	static const Shader *FindShader(const char *shaderName);
};