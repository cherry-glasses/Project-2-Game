#include "Application.h"
#include "ChooseMap.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Language.h"
#include "ModuleTransitionManager.h"



ChooseMap::ChooseMap(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{
	map1 = { _config.child("map1").attribute("x").as_int(), _config.child("map1").attribute("y").as_int(), 
		_config.child("map1").attribute("w").as_int(), _config.child("map1").attribute("h").as_int() };
	map2 = { _config.child("map2").attribute("x").as_int(), _config.child("map2").attribute("y").as_int(),
		_config.child("map2").attribute("w").as_int(), _config.child("map2").attribute("h").as_int() };
	map3 = { _config.child("map3").attribute("x").as_int(), _config.child("map3").attribute("y").as_int(),
		_config.child("map3").attribute("w").as_int(), _config.child("map3").attribute("h").as_int() };
	map4 = { _config.child("map4").attribute("x").as_int(), _config.child("map4").attribute("y").as_int(),
		_config.child("map4").attribute("w").as_int(), _config.child("map4").attribute("h").as_int() };

	press_sfx = App->audio->LoadFx(_config.child("press_fx_name").attribute("source").as_string());
}

// Destructor
ChooseMap::~ChooseMap()
{}


// Called before the first frame
bool ChooseMap::Start()
{
	maps_texture = App->textures->Load("Assets/Sprites/UI/UI_sprites.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();
	alpha = { 0, 0, screen_width, screen_height };

	App->audio->PlayMusic("Main_menu_8_bits.ogg");
	map_selected = 1;
	CreateMenu();
	return true;
}

// Called before all Updates
bool ChooseMap::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ChooseMap::Update(float _dt)
{
	bool ret = true;

	NavigateMaps();
	
	if (map_selected != 2) App->render->Blit(maps_texture, (screen_width / 2) - (map2.w/2), (screen_height / 2) - map2.h, &map2);
	if (map_selected != 4) App->render->Blit(maps_texture, (screen_width / 2) - (map2.w / 2), (screen_height / 2) - (map4.h / 5), &map4);
	if (map_selected != 3) App->render->Blit(maps_texture, (screen_width / 2) - (map3.w / 2) - (map3.w / 5), (screen_height / 2), &map3);
	if (map_selected != 1) App->render->Blit(maps_texture, (screen_width / 2) - map1.w - (map1.w/5), (screen_height / 2) - (map1.h / 2), &map1);
	
	App->render->DrawQuad(alpha, 0, 0, 0, 180);
	switch (map_selected)
	{
	case 1:
		App->render->Blit(maps_texture, (screen_width / 2) - map1.w - (map1.w / 5), (screen_height / 2) - (map1.h / 2), &map1);
		break;
	case 2:
		App->render->Blit(maps_texture, (screen_width / 2) - (map2.w / 2), (screen_height / 2) - map2.h, &map2);
		break;
	case 3:
		App->render->Blit(maps_texture, (screen_width / 2) - (map3.w / 2) - (map3.w / 5), (screen_height / 2), &map3);
		break;
	case 4:
		App->render->Blit(maps_texture, (screen_width / 2) - (map2.w / 2), (screen_height / 2) - (map4.h / 5), &map4);
		break;
	}

	if (App->input->Accept() && !App->scene_manager->changing)
	{
		switch (map_selected)
		{
		case 1:
			App->transition_manager->CreateSquaresTransition(2, true, FIRST_BATTLE, Scene_1_color);
			App->scene_manager->changing = true;
			break;
		case 2:
			App->transition_manager->CreateSquaresTransition(2, true, SECOND_BATTLE, Scene_2_color);
			App->scene_manager->changing = true;
			break;
		case 3:
			App->transition_manager->CreateSquaresTransition(2, true, THIRD_BATTLE, Scene_3_color);
			App->scene_manager->changing = true;
			break;
		case 4:
			App->transition_manager->CreateSquaresTransition(2, true, FOURTH_BATTLE, Scene_4_color);
			App->scene_manager->changing = true;
			break;
		}
	}
	if (App->input->Decline() && !App->scene_manager->changing)
	{
		App->transition_manager->CreateFadeTransition(2, true, MAIN_MENU, Black);
		App->scene_manager->changing = true;
	}

	return ret;
}

// Called before all Updates
bool ChooseMap::PostUpdate(float dt)
{
	return true;
}

// Called before quitting
bool ChooseMap::CleanUp()
{
	App->textures->UnLoad(menu_background);
	App->textures->UnLoad(maps_texture);
	App->audio->UnloadAllFx();
	App->gui_manager->DeleteAllGUIElements();
	return true;
}


// Load
bool ChooseMap::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool ChooseMap::Save(pugi::xml_node& _data) const
{
	return true;
}

void ChooseMap::CreateMenu()
{
	switch (map_selected)
	{
	case 1:
		map1_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2) - (map1.w * 1.6), (screen_height / 2) + (map1.h/3), App->scene_manager->language->map1, { 194, 213, 231, 255 }, App->gui_manager->default_font_used);
		break;
	case 2:	
		map2_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), (screen_height / 2) - map2.h - (map2.h/10), App->scene_manager->language->map2, { 178, 2, 2, 255 }, App->gui_manager->default_font_used);
		break;
	case 3:
		map3_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2) - (map3.w / 5), (screen_height / 2) + map3.h + (map3.h/8), App->scene_manager->language->map3, { 226, 232, 234, 255 }, App->gui_manager->default_font_used);
		break;
	case 4:
		map4_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2) + map4.w, (screen_height / 2), App->scene_manager->language->map4, { 254, 0, 114, 255 }, App->gui_manager->default_font_used);
		break;
	}
}

void ChooseMap::NavigateMaps()
{
	if (App->input->AllMaps() && !debug_maps)
	{
		if (!App->scene_manager->battle1_passed)
		{
			App->scene_manager->battle1_passed = true;
			debug_map_1 = true;
		}
		if (!App->scene_manager->battle2_passed)
		{
			App->scene_manager->battle2_passed = true;
			debug_map_2 = true;
		}
		if (!App->scene_manager->battle3_passed)
		{
			App->scene_manager->battle3_passed = true;
			debug_map_3 = true;
		}
		debug_maps = true;
	}
	else if(App->input->AllMaps())
	{
		if (debug_map_1)
		{
			App->scene_manager->battle1_passed = false;
			debug_map_1 = false;
		}
		if (debug_map_2)
		{
			App->scene_manager->battle2_passed = false;
			debug_map_2 = false;
		}
		if (debug_map_3)
		{
			App->scene_manager->battle3_passed = false;
			debug_map_3 = false;
		}
		debug_maps = false;
	}
	if (App->input->Right())
	{
		if (map_selected == 1 && App->scene_manager->battle1_passed) {
			App->audio->PlayFx(press_sfx);
			map_selected = 3;
		}
		else if ((map_selected == 2 || map_selected == 3) && App->scene_manager->battle3_passed && App->scene_manager->battle2_passed)
		{
			App->audio->PlayFx(press_sfx);
			map_selected = 4;
		}
		App->gui_manager->DeleteAllGUIElements();
		CreateMenu();

	}
	else if (App->input->Left())
	{
		if (map_selected == 2 || map_selected == 3)
		{
			App->audio->PlayFx(press_sfx);
			map_selected = 1;
		}
		else if (map_selected == 4 )
		{
			App->audio->PlayFx(press_sfx);
			--map_selected;
		}
		App->gui_manager->DeleteAllGUIElements();
		CreateMenu();
	}
	else if (App->input->Up())
	{
		if ((map_selected == 1 && App->scene_manager->battle1_passed) || map_selected == 3 || map_selected == 4) {
			App->audio->PlayFx(press_sfx);
			map_selected = 2;
		}
		App->gui_manager->DeleteAllGUIElements();
		CreateMenu();
	}
	else if (App->input->Down())
	{
		if ((map_selected == 1 && App->scene_manager->battle1_passed) || map_selected == 2 || map_selected == 4) {
			App->audio->PlayFx(press_sfx);
			map_selected = 3;
		}
		App->gui_manager->DeleteAllGUIElements();
		CreateMenu();
	}
}