#include "StandardMaterial.h"

#define UNIFORM_MODEL			"u_Model"
#define UNIFORM_CAMERA_POSITION	"u_CameraPosition"
#define UNIFORM_CAMERA_SPACE	"u_CameraSpace"
#define UNIFORM_LIGHT_POSITION	"u_LightDirection"
#define UNIFORM_LIGHT_SPACE		"u_LightSpace"

#define UNIFORM_MAINCOLOR		"u_MainColor"
#define UNIFORM_SPECULARCOLOR	"u_SpecularColor"
#define UNIFORM_SMOOTHNESS		"u_Smoothness"

void StandardMaterial::InitUniformLocations()
{
	modelMatrixLoc = shader.GetUniformLocation(UNIFORM_MODEL);

	cameraPosLoc = shader.GetUniformLocation(UNIFORM_CAMERA_POSITION);
	cameraSpaceLoc = shader.GetUniformLocation(UNIFORM_CAMERA_SPACE);

	shadowLoc = shader.GetUniformLocation(TEXTURE_NAME_SHADOWMAP);

	lightDirLoc = shader.GetUniformLocation(UNIFORM_LIGHT_POSITION);
	lightSpaceLoc = shader.GetUniformLocation(UNIFORM_LIGHT_SPACE);

	mainColorLoc = shader.GetUniformLocation(UNIFORM_MAINCOLOR);
	specularLoc = shader.GetUniformLocation(UNIFORM_SPECULARCOLOR);
	smoothnessLoc = shader.GetUniformLocation(UNIFORM_SMOOTHNESS);
}

void StandardMaterial::SetModel(const Matrix4 & m)
{
	shader.SetMat4(modelMatrixLoc, m);
}

void StandardMaterial::SetCameraPosition(const Vector3 & pos)
{
	shader.SetVec3(cameraPosLoc, pos);
}

void StandardMaterial::SetCameraSpace(const Matrix4 & vp)
{
	shader.SetMat4(cameraSpaceLoc, vp);
}

void StandardMaterial::SetLightDirection(const Vector3 & pos)
{
	shader.SetVec3(lightDirLoc, pos);
}

void StandardMaterial::SetLightSpace(const Matrix4 & vp)
{
	shader.SetMat4(lightSpaceLoc, vp);
}

void StandardMaterial::ActivateShadowMap()
{
	shader.SetInt(shadowLoc, (int)TextureType::Shadowmap);
}
