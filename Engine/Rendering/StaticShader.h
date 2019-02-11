//#pragma once
//
//#include "Shader.h"
//
//// Shader for 3d models
//class StaticShader : public Shader
//{
//private:
//	int modelLocation;
//	int viewLocation;
//	int projectionLocation;
//
//protected:
//	virtual void BindAttributes() override;
//	virtual void InitUniformLocations() override;
//
//public:
//	// Constructor
//	StaticShader();
//
//	virtual void Init() override;
//
//	void LoadModelMatrix(const Matrix4 &model) const;
//	void LoadViewMatrix(const Matrix4 &view) const;
//	void LoadProjectionMatrix(const Matrix4 &projection) const;
//};