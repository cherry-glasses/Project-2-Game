#include "Application.h"
#include "ModuleSceneManager.h"
#include "MainMenu.h"
#include "Battle1.h"
#include "Battle2.h"
#include "Battle3.h"
#include "Battle4.h"
#include "Win.h"
#include "Lose.h"
#include "ChooseMap.h"
#include "Intro.h"
#include "ModuleInput.h"



ModuleSceneManager::ModuleSceneManager()
{
	name = "scene_manager";
}

ModuleSceneManager::~ModuleSceneManager()
{

}

bool ModuleSceneManager::Awake(pugi::xml_node & _config)
{
	scene_configs = _config;
	current_scene = new MainMenu(SCENE_TYPE::MAIN_MENU, _config);
	return true;
}

bool ModuleSceneManager::Start()
{
	language = new Language();
	App->input->Defaultcontrols();
	if (i_want_a_trailer)
		return true;
	return current_scene->Start();
}

bool ModuleSceneManager::PreUpdate()
{
	if (i_want_a_trailer)
		return true;
	if (trailer == false) {
		current_scene->Start();
		trailer = true;
	}
	return current_scene->PreUpdate();
}

bool ModuleSceneManager::Update(float dt)
{
	if (i_want_a_trailer)
		return true;
	return current_scene->Update(dt);
}

bool ModuleSceneManager::PostUpdate(float _dt)
{
	if (i_want_a_trailer)
		return true;
	return current_scene->PostUpdate(_dt);
}

bool ModuleSceneManager::CleanUp()
{
	if (i_want_a_trailer)
		return true;
	return current_scene->CleanUp();
}

// Load
bool ModuleSceneManager::Load(pugi::xml_node& _data)
{
	battle1_passed = _data.child("battle1").attribute("value").as_bool();
	battle2_passed = _data.child("battle2").attribute("value").as_bool();
	battle3_passed = _data.child("battle3").attribute("value").as_bool();

	return true;
}

//Save
bool ModuleSceneManager::Save(pugi::xml_node& _data) const
{
	_data.append_child("battle1").append_attribute("value") = battle1_passed;
	_data.append_child("battle2").append_attribute("value") = battle2_passed;
	_data.append_child("battle3").append_attribute("value") = battle3_passed;

	return true;
}

//Scene unloads current scene, then creates and loads new scene.
void ModuleSceneManager::ChangeScene(SCENE_TYPE _type)
{
	current_scene->CleanUp();
	
	delete current_scene;
	current_scene = nullptr;

	switch (_type)
	{
	case MAIN_MENU:
		current_scene = new MainMenu(_type, scene_configs);
		break;
	case FIRST_BATTLE:
		current_scene = new Battle1(_type, scene_configs);
		break;
	case SECOND_BATTLE:
		current_scene = new Battle2(_type, scene_configs);
		break;
	case THIRD_BATTLE:
		current_scene = new Battle3(_type, scene_configs);
		break;
	case FOURTH_BATTLE:
		current_scene = new Battle4(_type, scene_configs);
		break;
	case WIN_SCENE:
		current_scene = new Win(_type, scene_configs);
		break;
	case LOSE_SCENE:
		current_scene = new Lose(_type, scene_configs);
		break;
	case CHOOSE_MAP:
		current_scene = new ChooseMap(_type, scene_configs);
		break;
	case INTRO:
		current_scene = new Intro(_type, scene_configs);
		break;
	case NONE:
		break;
	default:
		break;
	}

	current_scene->Start();
	changing = false;
}