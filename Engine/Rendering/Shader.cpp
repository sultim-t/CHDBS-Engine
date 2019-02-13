#include "Shader.h"
#include "OpenGL.h"

#include <Engine/Math/Vector.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Systems/RenderingSystem.h>

#include <fstream>
#include <sstream>
#include <iostream>

#pragma region default shader strings
const char* vertexDefaultShader =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 inTexCoord;\n"
"out vec2 texCoord;"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"	texCoord = vec2(inTexCoord.x, 1.0 - inTexCoord.y);\n"
"}\0";

const char* fragmentDefaultShader =
"#version 330 core\n"
"out vec4 fragColor;\n"
"in vec2 texCoord;\n"
"uniform sampler2D mainTexture;;\n"
"void main()\n"
"{\n"
"	fragColor = texture(mainTexture, texCoord);\n"
"}\n\0";
#pragma endregion

Shader::Shader()
{ }

Shader::~Shader()
{
	Stop();

	glDetachShader(graphicsProgramId, vertId);
	glDetachShader(graphicsProgramId, fragId);

	glDeleteShader(vertId);
	glDeleteShader(fragId);

	glDeleteProgram(graphicsProgramId);
}

void Shader::Init()
{
	RenderingSystem::Instance().Register(this);
}

void Shader::Load(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file’s buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		printf("SHADERS: Can't read shader code\n");
	}

	const char* vertexShader = vertexCode.c_str();
	const char* fragmentShader = fragmentCode.c_str();

	int success;

	// vertex shader
	vertId = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertId, 1, &vertexShader, NULL);
	glCompileShader(vertId);

	// check errors
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		printf("Vertex shader compilation error!\n");
	}

	// fragment shader
	fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &fragmentShader, NULL);
	glCompileShader(fragId);

	// check errors
	glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		printf("Vertex fragment compilation error!\n");
	}

	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
	}

	// shader Program
	graphicsProgramId = glCreateProgram();
	glAttachShader(graphicsProgramId, vertId);
	glAttachShader(graphicsProgramId, fragId);
	if (geometryPath != nullptr)
	{
		glAttachShader(graphicsProgramId, geometry);
	}

	glLinkProgram(graphicsProgramId);

	glDeleteShader(vertId);
	glDeleteShader(fragId);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);

	BindAttributes();
	InitUniformLocations();
}

void Shader::BindAttribute(int attribute, const char * name) const
{
	glBindAttribLocation(graphicsProgramId, attribute, name);
}

int Shader::GetUniformLocation(const char * name) const
{
	return glGetUniformLocation(graphicsProgramId, name);
}

void Shader::Use() const
{
	glUseProgram(graphicsProgramId);
}

void Shader::Stop() const
{
	glUseProgram(0);
}

void Shader::SetBool(const char * name, bool value) const
{
	glUniform1i(glGetUniformLocation(graphicsProgramId, name), (int)value);
}

void Shader::SetInt(const char * name, int value) const
{
	glUniform1i(glGetUniformLocation(graphicsProgramId, name), value);
}

void Shader::SetFloat(const char * name, float value) const
{
	glUniform1f(glGetUniformLocation(graphicsProgramId, name), value);
}

void Shader::SetVec2(const char * name, const Vector2 &vec) const
{
	glUniform2fv(glGetUniformLocation(graphicsProgramId, name), 1, vec.ToArray());
}

void Shader::SetVec2(const char * name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(graphicsProgramId, name), x, y);
}

void Shader::SetVec3(const char * name, const Vector3 &vec) const
{
	glUniform3fv(glGetUniformLocation(graphicsProgramId, name), 1, vec.ToArray());
}

void Shader::SetVec3(const char * name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(graphicsProgramId, name), x, y, z);
}

void Shader::SetVec4(const char * name, const Vector4 &vec) const
{
	glUniform4fv(glGetUniformLocation(graphicsProgramId, name), 1, vec.ToArray());
}

void Shader::SetVec4(const char * name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(graphicsProgramId, name), x, y, z, w);
}

void Shader::SetMat2(const char * name, const Matrix2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(graphicsProgramId, name), 1, GL_FALSE, mat.ToArray());
}

void Shader::SetMat3(const char * name, const Matrix3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(graphicsProgramId, name), 1, GL_FALSE, mat.ToArray());
}

void Shader::SetMat4(const char * name, const Matrix4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(graphicsProgramId, name), 1, GL_FALSE, mat.ToArray());
}

int Shader::GetProgramID() const
{
	return graphicsProgramId;
}

bool Shader::IsAffectedByLight() const
{
	return isAffectedByLight;
}

bool Shader::Is3D() const
{
	return is3D;
}
