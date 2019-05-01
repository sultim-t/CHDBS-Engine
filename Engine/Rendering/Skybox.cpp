#include "Skybox.h"
#include "OpenGL.h"

#define SKYBOX_SHADER_CUBEMAP_NAME		"skybox"
#define SKYBOX_SHADER_CAMERASPACE_NAME	"camSpace"

Skybox::Skybox() : isCubemapSet(false)
{ }

Skybox::~Skybox()
{ }

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
}

void Skybox::BindShader(const char *name)
{
	skyboxShader = Shader::FindShader(name);

	// set cubmap
	skyboxShader->Use();
	skyboxShader->SetInt(SKYBOX_SHADER_CUBEMAP_NAME, 0);
	skyboxShader->Stop();
}

void Skybox::BindCubemap(const Cubemap &cubemap)
{
	this->cubemap = cubemap;
	this->isCubemapSet = true;
}

void Skybox::Draw(const Matrix4 &camSpace) const
{
	// no cubemap
	if (!isCubemapSet)
	{
		return;
	}

	glDepthFunc(GL_LEQUAL);

	// set shader
	skyboxShader->Use();
	// set uniforms
	skyboxShader->SetMat4(SKYBOX_SHADER_CAMERASPACE_NAME, camSpace);
	// activate cubemap
	cubemap.Activate(0);

	// bind
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// to default
	skyboxShader->Stop();
	cubemap.Deactivate();
	glDepthFunc(GL_LESS);
}

Skybox &Skybox::Instance()
{
	static Skybox skybox;
	return skybox;
}
