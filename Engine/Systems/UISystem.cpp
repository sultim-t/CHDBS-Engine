#include "UISystem.h"

void UISystem::Init()
{
	elements.Init(16, 4);
	elements.DeclareHashFunction(String::StringHash);

	//material.BindShader("UI");
	//material.InitUniformLocations();
}

void UISystem::Update()
{
	UINT size = elements.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		UIElement *elem = elements[i];

		if (!elem->IsActive)
		{
			continue;
		}

		elem->Draw();
	}
}

UISystem &UISystem::Instance()
{
	static UISystem instance;
	return instance;
}

UISystem::~UISystem()
{
	// delete each element
	UINT size = elements.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		delete elements[i];
	}
}

UIText *UISystem::AddText(const char *name)
{
	UIText *textElem = new UIText(name);
	elements.Add(name, textElem);

	return textElem;
}

void *UISystem::AddImage(const char *name)
{
	return nullptr;
}

void UISystem::Remove(const char *name)
{
	UIElement *removed = nullptr;
	elements.Remove(name, removed);

	if (removed != nullptr)
	{
		delete removed;
	}
}

UIElement *UISystem::Find(const char *name)
{
	UIElement *found = nullptr;
	elements.Find(name, found);

	return found;
}
