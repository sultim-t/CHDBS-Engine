#pragma once

class Shader
{
	friend class Material;
	friend class RenderingSystem;

private:
	ShaderID shaderId;

	// Program ID
	UINT graphicsProgramId;
	UINT vertId;
	UINT fragId;
	UINT geomId;

public:
	// Reads and builds the shader
	void Load(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);
	// Load shader from strings
	void LoadFromStrings(const char *vertex, const char *fragment, const char *geometry = nullptr);

	// Bind attribute
	void BindAttribute(int attribute, const char *name) const;
	// Bind attributes' indices
	virtual void BindAttributes() {};

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
	void SetMat4(int location, const Matrix4 &mat) const;

public:
	Shader();
	// Destructor
	~Shader();

	// Register shader in rendering system
	virtual void Init();

	// Use/activate the shader
	void Use() const;
	// Deactivate
	void Stop() const;

	int GetProgramID() const;
};