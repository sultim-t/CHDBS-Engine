#include "StandardMaterial.h"
#include <Engine/Rendering/Cubemap.h>

#define UNIFORM_MODEL			"u_Model"
#define UNIFORM_CAMERA_POSITION	"u_CameraPosition"
#define UNIFORM_CAMERA_SPACE	"u_CameraSpace"
#define UNIFORM_LIGHT_COUNT		"u_LightCount"
#define UNIFORM_LIGHT_SPACE		"u_LightSpace"

#define UNIFORM_MAINCOLOR		"u_MainColor"
#define UNIFORM_SPECULARCOLOR	"u_SpecularColor"
#define UNIFORM_SMOOTHNESS		"u_Smoothness"

#define UNIFORM_LIGHT_POS		"u_Lights[i].Position"
#define UNIFORM_LIGHT_COL		"u_Lights[i].Color"
#define UNIFORM_LIGHT_ATTEN		"u_Lights[i].Attenuation"
#define UNIFORM_LIGHT_CANGLE	"u_Lights[i].ConeAngle"
#define UNIFORM_LIGHT_CDIR		"u_Lights[i].ConeDirection"
#define UNIFORM_LIGHT_INDEXPOS	9

void StandardMaterial::InitUniformLocations()
{
	modelMatrixLoc = shader->GetUniformLocation(UNIFORM_MODEL);

	cameraPosLoc = shader->GetUniformLocation(UNIFORM_CAMERA_POSITION);
	cameraSpaceLoc = shader->GetUniformLocation(UNIFORM_CAMERA_SPACE);

	shadowLoc = shader->GetUniformLocation(TEXTURE_NAME_SHADOWMAP);

	lightCountLoc = shader->GetUniformLocation(UNIFORM_LIGHT_COUNT);
	lightSpaceLoc = shader->GetUniformLocation(UNIFORM_LIGHT_SPACE);

	mainColorLoc = shader->GetUniformLocation(UNIFORM_MAINCOLOR);
	specularLoc = shader->GetUniformLocation(UNIFORM_SPECULARCOLOR);
	smoothnessLoc = shader->GetUniformLocation(UNIFORM_SMOOTHNESS);

	InitLightUniforms();
}

void StandardMaterial::InitLightUniforms()
{
	// create copies
	String position = UNIFORM_LIGHT_POS;
	String color = UNIFORM_LIGHT_COL;
	String atten = UNIFORM_LIGHT_ATTEN;
	String cangle = UNIFORM_LIGHT_CANGLE;
	String cdir = UNIFORM_LIGHT_CDIR;

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		char index = '0' + i;

		// deform names according to their index
		position[UNIFORM_LIGHT_INDEXPOS] = index;
		color[UNIFORM_LIGHT_INDEXPOS] = index;
		atten[UNIFORM_LIGHT_INDEXPOS] = index;
		cangle[UNIFORM_LIGHT_INDEXPOS] = index;
		cdir[UNIFORM_LIGHT_INDEXPOS] = index;

		// get uniforms
		lightUniforms[i].positionLoc = shader->GetUniformLocation(position);
		lightUniforms[i].colorLoc = shader->GetUniformLocation(color);
		lightUniforms[i].attenLoc = shader->GetUniformLocation(atten);
		lightUniforms[i].coneAngleLoc = shader->GetUniformLocation(cangle);
		lightUniforms[i].coneDirLoc = shader->GetUniformLocation(cdir);
	}
}

void StandardMaterial::ActivateAdditional() const
{
	int type = (int)reflectionCubemap->GetType();

	// bind to correct texture unit
	shader->SetInt(TEXTURE_NAME_CUBEMAP, type);

	// activate
	reflectionCubemap->Activate(type);
}

void StandardMaterial::DeactivateAdditional() const
{
	reflectionCubemap->Deactivate();
}

void StandardMaterial::SetModel(const Matrix4 & m)
{
	shader->SetMat4(modelMatrixLoc, m);
}

void StandardMaterial::SetCameraPosition(const Vector3 & pos)
{
	shader->SetVec3(cameraPosLoc, pos);
}

void StandardMaterial::SetCameraSpace(const Matrix4 & vp)
{
	shader->SetMat4(cameraSpaceLoc, vp);
}

void StandardMaterial::SetLightCount(int count)
{
	shader->SetInt(lightCountLoc, count);
}

void StandardMaterial::SetLightSpace(const Matrix4 & vp)
{
	shader->SetMat4(lightSpaceLoc, vp);
}

void StandardMaterial::SetReflectionCubemap(const Cubemap *cubemap)
{
	reflectionCubemap = cubemap;
}

void StandardMaterial::SetMainColor(const Color4F & c)
{
}

void StandardMaterial::SetSpecularColor(const Color4F & c)
{
}

void StandardMaterial::SetSmoothness(float s)
{
}

void StandardMaterial::SetLight(const ILight &light, int index)
{
	// set light's properties to shader
	// using calculated uniform locations
	shader->SetVec4(lightUniforms[index].positionLoc, light.GetPosition());
	shader->SetVec3(lightUniforms[index].colorLoc, light.GetColor());
	shader->SetFloat(lightUniforms[index].attenLoc, light.GetAttenuation());
	shader->SetFloat(lightUniforms[index].coneAngleLoc, light.GetConeAngle());
	shader->SetVec3(lightUniforms[index].coneDirLoc, light.GetConeDirection());
}

void StandardMaterial::ActivateShadowMap()
{
	shader->SetInt(shadowLoc, (int)TextureType::Shadowmap);
}
