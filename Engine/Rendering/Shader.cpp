#include "Shader.h"
#include "OpenGL.h"

#include <Engine/Math/Vector.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Systems/RenderingSystem.h>

#include <fstream>
#include <sstream>
#include <iostream>

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
		Logger::Print("Shaders::Can't read shader code");
	}
	
	if (geometryPath == nullptr)
	{
		LoadFromStrings(vertexCode.c_str(), fragmentCode.c_str(), nullptr);
	}
	else
	{
		LoadFromStrings(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.c_str());
	}
}

void Shader::LoadFromStrings(const char * vertex, const char * fragment, const char * geometry)
{
	char log[256];
	int success;

	// vertex shader
	vertId = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertId, 1, &vertex, NULL);
	glCompileShader(vertId);

	// check errors
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		Logger::Print("Shaders::Vertex shader compilation error");

		glGetShaderInfoLog(vertId, 256, NULL, log);
		Logger::Print(log);
	}

	// fragment shader
	fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &fragment, NULL);
	glCompileShader(fragId);

	// check errors
	glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		Logger::Print("Shaders::Fragment shader compilation error");

		glGetShaderInfoLog(fragId, 256, NULL, log);
		Logger::Print(log);
	}

	if (geometry != nullptr)
	{
		geomId = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geomId, 1, &geometry, NULL);
		glCompileShader(geomId);
	}

	// shader Program
	graphicsProgramId = glCreateProgram();
	glAttachShader(graphicsProgramId, vertId);
	glAttachShader(graphicsProgramId, fragId);
	if (geometry != nullptr)
	{
		glAttachShader(graphicsProgramId, geomId);
	}

	glLinkProgram(graphicsProgramId);

	glDeleteShader(vertId);
	glDeleteShader(fragId);
	if (geometry != nullptr)
	{
		glDeleteShader(geomId);
	}
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

void Shader::SetInt(int location, int value) const
{
	glUniform1i(location, value);
}

void Shader::SetFloat(int location, float value) const
{
	glUniform1f(location, value);
}

void Shader::SetVec3(int location, const Vector3 & vec) const
{
	glUniform3fv(location, 1, vec.ToArray());
}

void Shader::SetMat4(int location, const Matrix4 & mat) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.ToArray());
}

int Shader::GetProgramID() const
{
	return graphicsProgramId;
}