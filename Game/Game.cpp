#include "Game.h"

#include <Engine/Entities/EntityFactory.h>
#include "Scripts/Weapon.h"

int main()
{
	// load global settings
	Engine engine = Engine();
	engine.Init("GlobalSettings.xml");

	// register user component types
	LoadComponentTypes();

	// start main loop
	engine.Start();

	return 0;
}

void LoadComponentTypes()
{
	EntityFactory::RegisterComponentType<CWeapon>("CWeapon");
}