#pragma once

#include "Cubemap.h"
#include "Shader.h"

class Skybox
{
private:
	UINT vao, vbo;
	Cubemap cubemap;
	Shader shader;

	bool isCubemapSet;

private:
	Skybox();
	~Skybox();

	Skybox(Skybox&) = delete;
	Skybox(Skybox&&) = delete;
	Skybox &operator=(const Skybox&) = delete;

public:
	void Init();
	void BindCubemap(const Cubemap &cubemap);
	void Draw(const Matrix4 &camSpace) const;

	static Skybox &Instance();
};