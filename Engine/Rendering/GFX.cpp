//#include "GFX.h"
//#include "OpenGL.h"
//
//namespace Graphics
//{
//	extern void(*gfxEnableDepthTest)() = NULL;
//	extern void(*gfxDisableDepthTest)() = NULL;
//
//	extern void(*gfxPolygonMode)(GFXPolygonMode polygonMode) = NULL;
//
//	extern void(*gfxEnableTexture)() = NULL;
//	extern void(*gfxDisableTexture)() = NULL;
//	extern void(*gfxGenerateTexture)(UINT &texObject) = NULL;
//	extern void(*gfxDeleteTexture)(UINT &texObject) = NULL;
//	extern void(*gfxSetTextureWrapping)(GFXWrap wrapU, GFXWrap wrapV) = NULL;
//
//	extern void(*gfxSetVertexArray)(GFXVertex4 *verts, UINT vertCount) = NULL;
//	extern void(*gfxSetNormalArray)(GFXNormal3 *normals) = NULL;
//	extern void(*gfxSetTexCoordArray)(GFXTexCoord *tex) = NULL;
//
//	extern void(*gfxDrawElements)(UINT *indices, UINT elemCount) = NULL;
//
//	void SetGFXAPI(GFXAPI api)
//	{
//		if (api == GFX_API_OpenGL)
//		{
//			using namespace GraphicsOpenGL;
//
//			gfxEnableDepthTest = oglEnableDepthTest;
//			gfxDisableDepthTest = oglDisableDepthTest;
//
//			gfxPolygonMode = oglPolygonMode;
//
//			gfxEnableTexture = oglEnableTexture;
//			gfxDisableTexture = oglDisableTexture;
//			gfxGenerateTexture = oglGenerateTexture;
//			gfxDeleteTexture = oglDeleteTexture;
//			gfxSetTextureWrapping = oglSetTextureWrapping;
//
//			gfxSetVertexArray = oglSetVertexArray;
//			gfxSetNormalArray = oglSetNormalArray;
//			gfxSetTexCoordArray = oglSetTexCoordArray;
//
//			gfxDrawElements = oglDrawElements;
//		}
//		else
//		{
//			// no api
//		}
//	}
//}
