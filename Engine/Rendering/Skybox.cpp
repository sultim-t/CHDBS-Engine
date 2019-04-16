#include "Skybox.h"
#include "OpenGL.h"

Skybox::Skybox()
{
	isCubemapSet = false;
}

Skybox::~Skybox()
{
}

void Skybox::Init()
{
	// skybox always is a box
	float skyboxVertices[] = {    
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	shader.Load("Shaders/Skybox.vs", "Shaders/Skybox.fs");
	shader.Use();
	shader.SetInt("skybox", 0);
	shader.Stop();
}

void Skybox::BindCubemap(const Cubemap &cubemap)
{
	this->cubemap = cubemap;
	isCubemapSet = true;
}

void Skybox::Draw(const Matrix4 &camSpace) const
{
	if (!isCubemapSet)
	{
		return;
	}

	glDepthFunc(GL_LEQUAL);

	// set shader
	shader.Use();

	// set uniforms
	shader.SetMat4("camSpace", camSpace);
	
	// activate cubemap
	cubemap.Activate(0);

	// bind
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// to default
	glDepthFunc(GL_LESS);
}

Skybox &Skybox::Instance()
{
	static Skybox skybox;
	return skybox;
}
