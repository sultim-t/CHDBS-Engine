#pragma once

class ResourceManager
{
private:
public:
	static UBYTE *LoadTexture(char const *filename, INT *width, INT *height, INT *comp, INT req_comp);
	static void DeleteTexture(void *address);

	static void LoadModel();
	static void LoadText();
	// void LoadXML();
};