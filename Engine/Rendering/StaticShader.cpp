//#include "StaticShader.h"
//
//#define VERT_PATH "Shaders\\TexturedMvp.vs"
//#define FRAG_PATH "Shaders\\TexturedMvp.fs"
//
//#define ATTRIB_POSITION "position"
//#define ATTRIB_TEXCOORD "texCoords"
//
//#define UNIFORM_MODEL "model"
//#define UNIFORM_VIEW "view"
//#define UNIFORM_PROJ "projection"
//
//StaticShader::StaticShader()
//{ }
//
//void StaticShader::LoadModelMatrix(const Matrix4& model) const
//{
//	SetMat4(UNIFORM_MODEL, model);
//}
//
//void StaticShader::LoadViewMatrix(const Matrix4& view) const
//{
//	SetMat4(UNIFORM_VIEW, view);
//}
//
//void StaticShader::LoadProjectionMatrix(const Matrix4& projection) const
//{
//	SetMat4(UNIFORM_PROJ, projection);
//}
//
//void StaticShader::BindAttributes()
//{
//	BindAttribute(0, ATTRIB_POSITION);
//	BindAttribute(1, ATTRIB_TEXCOORD);
//}
//
//void StaticShader::InitUniformLocations()
//{
//	modelLocation = GetUniformLocation(UNIFORM_MODEL);
//	viewLocation = GetUniformLocation(UNIFORM_VIEW);
//	projectionLocation = GetUniformLocation(UNIFORM_PROJ);
//}
//
//void StaticShader::Init()
//{
//	Load(VERT_PATH, FRAG_PATH);
//}
