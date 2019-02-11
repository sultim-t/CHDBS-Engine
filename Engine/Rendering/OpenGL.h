#pragma once

#define GLEW_STATIC
#include <GLEW\glew.h>


//#include "GFX.h"
//
//namespace GraphicsOpenGL
//{
//	using namespace Graphics;
//
//	static void oglEnableDepthTest();
//	static void oglDisableDepthTest();
//
//	static void oglPolygonMode(GFXPolygonMode polygonMode);
//
//	static void oglEnableTexture();
//	static void oglDisableTexture();
//	static void oglGenerateTexture(UINT &texObject);
//	static void oglDeleteTexture(UINT &texObject);
//	static void oglSetTextureWrapping(GFXWrap wrapU, GFXWrap wrapV);
//
//	static void oglSetVertexArray(GFXVertex4 *verts, UINT vertCount);
//	static void oglSetNormalArray(GFXNormal3 *normals);
//	static void oglSetTexCoordArray(GFXTexCoord *tex);
//	static void oglSetColorArray(GFXColor *colors);
//
//	static void oglDrawElements(UINT *indices, UINT elemCount);
//
//}