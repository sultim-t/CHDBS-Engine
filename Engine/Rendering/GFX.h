//#pragma once
//
//namespace Graphics
//{
//	enum GFXAPI
//	{
//		GFX_API_OpenGL
//	};
//
//	enum GFXWrap
//	{
//		GFX_REPEAT,
//		GFX_CLAMP,
//	};
//
//	enum GFXPolygonMode
//	{
//		GFX_POINT,
//		GFX_LINE,
//		GFX_FILL
//	};
//
//	struct GFXColor
//	{
//		UBYTE R, G, B, A;
//	};
//
//	struct GFXVertex3
//	{
//		FLOAT X, Y, Z;
//	};
//
//	struct GFXVertex4
//	{
//		FLOAT X, Y, Z;
//		GFXColor Color;
//	};
//
//	struct GFXNormal3
//	{
//		FLOAT X, Y, Z;
//	};
//
//	struct GFXTexCoord
//	{
//		FLOAT U, V;
//	};
//
//	extern void(*gfxEnableDepthTest)();
//	extern void(*gfxDisableDepthTest)();
//
//	extern void(*gfxPolygonMode)(GFXPolygonMode polygonMode);
//	
//	extern void(*gfxEnableTexture)();
//	extern void(*gfxDisableTexture)();
//	extern void(*gfxGenerateTexture)(UINT &texObject);
//	extern void(*gfxDeleteTexture)(UINT &texObject);
//	extern void(*gfxSetTextureWrapping)(GFXWrap wrapU, GFXWrap wrapV);
//
//	extern void(*gfxSetVertexArray)(GFXVertex4 *verts, UINT vertCount);
//	extern void(*gfxSetNormalArray)(GFXNormal3 *normals);
//	extern void(*gfxSetTexCoordArray)(GFXTexCoord *tex);
//	extern void(*gfxSetColorArray)(GFXColor *colors);
//	
//	extern void(*gfxDrawElements)(UINT *indices, UINT elemCount);
//
//	extern void SetGFXAPI(GFXAPI api);
//}