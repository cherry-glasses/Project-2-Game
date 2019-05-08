#include "Application.h"
#include "Stone.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"


Stone::Stone(ENTITY_TYPE _type, pugi::xml_node _config, int _copy) : Object(_type, _config)
{
	entity_texture = App->textures->Load("Assets/Sprites/Objects/stone.png");
	switch (_copy) {
	case 0:
		position = App->map->MapToWorld(3, 24);
		break;
	case 1:
		position = App->map->MapToWorld(0, 15);
		break;
	case 2:
		position = App->map->MapToWorld(0, 13);
		break;
	case 3:
		position = App->map->MapToWorld(0, 11);
		break;
	case 4:
		position = App->map->MapToWorld(0, 9);
		break;

	default:
		break;
	}
	current = { 0 , 0, 96, 128 };
}
Stone::~Stone()
{
}



