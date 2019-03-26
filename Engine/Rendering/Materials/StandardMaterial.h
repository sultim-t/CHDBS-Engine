#pragma once

#include <Engine/Math/Matrix.h>
#include <Engine/Rendering/Material.h>

// Represents material which is affected by light in 3D space
class StandardMaterial : public Material
{
private:
	// Model transformation shader location
	int modelMatrixLoc;

	// Camera position shader location
	int cameraPosLoc;
	// Camera space shader location
	int cameraSpaceLoc;
	
	// Shadowmap shader location
	int shadowLoc;

	// Main color shader location
	int mainColorLoc;
	// Specular color shader location
	int specularLoc;
	// Smoothness (float) shader location
	int smoothnessLoc;


	// must be light class
	int lightDirLoc;
	int lightSpaceLoc;

public:
	void InitUniformLocations();

	void SetModel(const Matrix4 &m);

	void SetCameraPosition(const Vector3 &pos);
	void SetCameraSpace(const Matrix4 &vp);

	void SetLightDirection(const Vector3 &pos);
	void SetLightSpace(const Matrix4 & vp);

	void SetMainColor(const Color4 & c);
	void SetSpecularColor(const Color4 & c);
	void SetSmoothness(float s);

	void ActivateShadowMap();
};