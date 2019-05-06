#pragma once

#define SHADER_NAME_STANDARD	"Standard"
#define SHADER_NAME_DEBUG		"DebugShader"
#define SHADER_NAME_SKYBOX		"SkyboxShader"
#define SHADER_NAME_TEXT		"TextShader"

class GlobalSettings
{
public:
	String Name;

	int ScreenWidth;
	int ScreenHeight;

	String StartScenePath;

	String StandardShaderVert;
	String StandardShaderFrag;

	String SkyboxShaderVert;
	String SkyboxShaderFrag;

	String DebugShaderVert;
	String DebugShaderFrag;

	String TextShaderVert;
	String TextShaderFrag;
};