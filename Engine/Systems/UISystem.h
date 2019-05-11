#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/HashTable.h>
#include <Engine/Rendering/GUI/UIElement.h>
#include <Engine/Rendering/GUI/UIText.h>
//#include <Engine/Rendering/Materials/UIMaterial.h>

class UISystem : public ISystem
{
private:
	// All UI elements to draw
	HashTable<String, UIElement*>	elements;
	// Main material for drawing
	//UIMaterial						material;

public:
	~UISystem();

	// Allocate text element
	UIText *AddText(const char *name);
	// Allocate image element
	void *AddImage(const char *name);

	// Remove UI element
	void Remove(const char *name);
	// Find Ui element
	UIElement *Find(const char *name);

	void Init() override;
	void Update() override;

	static UISystem &Instance();
};