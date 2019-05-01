#pragma once

#include "Cubemap.h"
#include "Shader.h"

class Skybox
{
private:
	UINT			vao, vbo;
	Cubemap			cubemap;
	const Shader	*skyboxShader;

	bool			isCubemapSet;

private:
	// Default constructor
	Skybox();
	// Default destructor
	~Skybox();

	Skybox(Skybox&) = delete;
	Skybox(Skybox&&) = delete;
	Skybox &operator=(const Skybox&) = delete;

public:
	void Init();
	void BindShader(const char *name);
	void BindCubemap(const Cubemap &cubemap);
	void Draw(const Matrix4 &camSpace) const;

	static Skybox &Instance();
};