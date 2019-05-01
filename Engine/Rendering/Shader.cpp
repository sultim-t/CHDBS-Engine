#include "Shader.h"
#include "OpenGL.h"

#include <Engine/Math/Vector.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>

Shader::Shader(const char *name) : name(name), isCompiled(false)
{ }

Shader::~Shader()
{
	// if not compiled
	if (!isCompiled)
	{
		// do nothing
		return;
	}

	// deactivate
	Stop();
	
	// unregister
	glDetachShader(graphicsProgramId, vertId);
	glDetachShader(graphicsProgramId, fragId);

	glDeleteShader(vertId);
	glDeleteShader(fragId);

	glDeleteProgram(graphicsProgramId);
}

void Shader::Load(const char *vertexPath, const char *fragmentPath)
{
	// if compiled
	if (isCompiled)
	{
		return;
	}

	// load shader
	resource = ResourceManager::Instance().LoadShader(vertexPath, fragmentPath);

	// compile shader
	Compile(resource->GetVertexCode(), resource->GetFragmentCode());
}

void Shader::Compile(const char *vertex, const char *fragment)
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
		Logger::Print("Shaders::Vertex shader compilation error in:");
		Logger::Print(resource->GetVertexPath());

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
		Logger::Print("Shaders::Fragment shader compilation error in:");
		Logger::Print(resource->GetFragmentPath());

		glGetShaderInfoLog(fragId, 256, NULL, log);
		Logger::Print(log);
	}

	// shader Program
	graphicsProgramId = glCreateProgram();
	glAttachShader(graphicsProgramId, vertId);
	glAttachShader(graphicsProgramId, fragId);

	glLinkProgram(graphicsProgramId);

	glDeleteShader(vertId);
	glDeleteShader(fragId);
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

void Shader::SetVec4(int location, const Vector4 & vec) const
{
	glUniform4fv(location, 1, vec.ToArray());
}

void Shader::SetMat4(int location, const Matrix4 & mat) const
{
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.ToArray());
}

int Shader::GetProgramID() const
{
	return graphicsProgramId;
}

const String &Shader::GetName() const
{
	return name;
}

const Shader *Shader::RegisterShader(const char *shaderName, const char *vertPath, const char *fragPath)
{
	// just a shortcut
	return RenderingSystem::Instance().RegisterShader(shaderName, vertPath, fragPath);
}

const Shader *Shader::FindShader(const char *shaderName)
{
	// just a shortcut
	return RenderingSystem::Instance().GetShader(shaderName);
}
