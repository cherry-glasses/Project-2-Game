#include "Log.h"
#include "Application.h"
#include "ModuleGUIManager.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "ModuleMap.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "Scene.h"
#include "ModuleEntityManager.h"
#include "ModuleWindow.h"
#include "CharacterIris.h"


Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}


// Called before render is available
bool Scene::Awake(pugi::xml_node& _config)
{
	LOG("Loading Scene");
	bool ret = true;

	button = { _config.child("button").attribute("x").as_int(), _config.child("button").attribute("y").as_int(),
		_config.child("button").attribute("w").as_int() , _config.child("button").attribute("h").as_int() };
	button_margin = _config.child("button").attribute("margin").as_int();
	cherry_glasses_logo = { _config.child("cherry_glasses_logo").attribute("x").as_int(), _config.child("cherry_glasses_logo").attribute("y").as_int(),
		_config.child("cherry_glasses_logo").attribute("w").as_int() , _config.child("cherry_glasses_logo").attribute("h").as_int() };
	option_background = { _config.child("option_background").attribute("x").as_int(), _config.child("option_background").attribute("y").as_int(),
		_config.child("option_background").attribute("w").as_int() , _config.child("option_background").attribute("h").as_int() };
	/*button_half_width = (_config.child("buttons").attribute("width").as_int()) * 0.5;
	button_half_height = (_config.child("buttons").attribute("height").as_int()) * 0.5;
	first_button_height = _config.child("main_menu").attribute("start_pos").as_int();
	options_first_height = _config.child("options_menu").attribute("start_pos").as_int();*/
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	main_menu_background = App->textures->Load("Assets/Sprites/UI/Main_menu_art_shot.png");
	options_background = App->textures->Load("Assets/Sprites/UI/4259708641.png");
	credits_page = App->textures->Load("Assets/Sprites/UI/credits_done.png");
	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();
	return true;
}

// Called before all Updates
bool Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::Update(float _dt)
{
	bool ret = true;

	switch (current_scene)
	{
	case Scene::MAIN_MENU:

		if (!main_menu_created) {
			main_menu_created = true;
			CreateMainMenu();
		}

		App->render->Blit(main_menu_background, 0, 0);

		if (new_game_button->has_been_clicked)
		{
			current_scene = FIRST_BATTLE;
			DeleteMenu();
		}
		else if (load_game_button->has_been_clicked)
		{
			current_scene = SECOND_BATTLE;
			DeleteMenu();
		}
		else if (options_button->has_been_clicked)
		{
			current_scene = MM_OPTIONS;
			DeleteMenu();
		}
		else if (credits_button->has_been_clicked )
		{
			current_scene = MM_CREDITS;
			DeleteMenu();
		}
		else if (exit_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
		else if (cherry_glasses_logo_button->has_been_clicked)
		{
			ShellExecuteA(NULL, "open", "https://github.com/cherry-glasses/Clowns-F8/wiki", NULL, NULL, SW_SHOWNORMAL);
			cherry_glasses_logo_button->Select(SELECTED);
		}
		Navigate();
		break;

	case Scene::GLOBAL_MAP:
		break;

	case Scene::MM_OPTIONS:
		App->render->Blit(main_menu_background, 0, 0);
		App->render->Blit(options_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));

		if (!mm_options_created)
		{
			mm_options_created = true;
			CreateMMOptions();
		}

		if (english_button->has_been_clicked)
		{
			if (!language)
			{
				language = true;
				DeleteMenu();
				CreateMMOptions();
			}
			english_button->Select(SELECTED);
		}
		else if (spanish_button->has_been_clicked)
		{
			if (language) 
			{
				language = false;
				DeleteMenu();
				CreateMMOptions();
				spanish_button->Select(NORMAL);
			}
			else spanish_button->Select(SELECTED);
		}
		else if (volume_up_button->has_been_clicked)
		{
			App->audio->VolumeUp();
			volume_up_button->Select(SELECTED);
		}
		else if (volume_down_button->has_been_clicked)
		{
			App->audio->VolumeDown();
			volume_down_button->Select(SELECTED);
		}
		else if (back_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			mm_options_created = false;
			current_scene = MAIN_MENU;
			DeleteMenu();
		}

		Navigate();
		break;

	case Scene::MM_CREDITS:
		App->render->Blit(main_menu_background, 0, 0);
		App->render->Blit(credits_page, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	
		if (mm_credits_created == false)
		{
			back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
			buttons.push_back(back_button);
			back_button->Select(SELECTED);
			if (language)
			{
				back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			}
			else
			{
				back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "ATR�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
			}
			mm_credits_created = true;
		}

		if (back_button->has_been_clicked || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			current_scene = MAIN_MENU;
			back_button->Select(SELECTED);
			mm_credits_created = false;
			DeleteMenu();
		}

		break;

	case Scene::FIRST_BATTLE:
		
		if (!map_loaded) {
			map_loaded = true;
			App->map->Load("map_level1.tmx");
			Sapphire = (CharacterSapphire*)App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
			Iris = (CharacterIris*)App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
			App->render->camera.x = App->window->GetScreenWidth() / 2;
			App->render->camera.y = App->window->GetScreenHeight() / 8;
			life_x = (124 * Iris->current_stats.Hp) / Iris->default_stats.Hp;
			mana_x = (124 * Iris->current_stats.Mana) / Iris->default_stats.Mana;
		}

		if (life_x != ((124 * Iris->current_stats.Hp) / Iris->default_stats.Hp) 
			|| mana_x != ((124 * Iris->current_stats.Mana) / Iris->default_stats.Mana) || !portraits_created)
		{
			if (portraits_created)
			{
				App->gui_manager->DeleteGUIElement(life);
				App->gui_manager->DeleteGUIElement(mana);
				App->gui_manager->DeleteGUIElement(portrait_1);
			}
			life_x = (124 * Iris->current_stats.Hp) / Iris->default_stats.Hp;
			mana_x = (124 * Iris->current_stats.Mana) / Iris->default_stats.Mana;
			life = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 150.0f, 79.0f, { 0, 58, life_x, 29 });
			mana = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 150.0f, 110.0f, { 0, 86, mana_x, 29 });
			portrait_1 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 12.0f, { 0, 115, 256, 128 });
		}

		if (!portraits_created)
		{
			iris_port = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 30.0f, 13.0f, { 124, 0, 64, 67 });
			portrait_1 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 12.0f, { 0, 115, 256, 128 });
			portrait_2 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, screen_height - 168, { 0, 115, 256, 128 });
			portrait_3 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, screen_width - 280, 12.0f, { 0, 115, 256, 128 });
			portrait_4 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, screen_width - 280, screen_height - 168, { 0, 115, 256, 128 });
			//action_menu = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 145.0f, { 0, 243, 256, 101 });
			portraits_created = true;
			action_menu_created = true;
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || waiting_for_input)
		{
			if (!waiting_for_input)
			{
				waiting_for_input = true;
				attack_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 20.0f, 145.0f, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons.push_back(attack_button);
				ability_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 20.0f, 184.0f, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons.push_back(ability_button);
				defend_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, 20.0f, 223.0f, { 288, 0, 173, 39 }, { 288, 39, 173, 39 }, { 288, 78, 173, 39 });
				buttons.push_back(defend_button);
				attack_button->Select(SELECTED);
			}
			Navigate();
			if (attack_button->has_been_clicked)
			{
				waiting_for_input = false;
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				buttons.clear();
			}
			else if (ability_button->has_been_clicked)
			{
				Iris->current_stats.Mana -= 10;
				waiting_for_input = false;
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				buttons.clear();
			}
			else if (defend_button->has_been_clicked)
			{
				waiting_for_input = false;
				App->gui_manager->DeleteGUIElement(attack_button);
				App->gui_manager->DeleteGUIElement(ability_button);
				App->gui_manager->DeleteGUIElement(defend_button);
				buttons.clear();
			}
		}
		// Camera Movment in map
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			App->render->camera.y += 4;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			App->render->camera.x += 4;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			App->render->camera.y -= 4;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			App->render->camera.x -= 4;
		
		
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
		break;
	case SECOND_BATTLE:
		if (!map_loaded) {
			map_loaded = true;
			App->map->Load("map_level2.tmx");
			Iris = (CharacterIris*)App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_CHARACTER_IRIS);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN);
			App->entity_manager->CreateEntity(ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
			App->render->camera.x = App->window->GetScreenWidth() / 2;
			App->render->camera.y = App->window->GetScreenHeight() / 8;
			life_x = (124 * Iris->current_stats.Hp) / Iris->default_stats.Hp;
			mana_x = (124 * Iris->current_stats.Mana) / Iris->default_stats.Mana;
		}

		if (life_x != ((124 * Iris->current_stats.Hp) / Iris->default_stats.Hp)
			|| mana_x != ((124 * Iris->current_stats.Mana) / Iris->default_stats.Mana) || !portraits_created)
		{
			if (portraits_created)
			{
				App->gui_manager->DeleteGUIElement(life);
				App->gui_manager->DeleteGUIElement(mana);
				App->gui_manager->DeleteGUIElement(portrait_1);
			}
			life_x = (124 * Iris->current_stats.Hp) / Iris->default_stats.Hp;
			mana_x = (124 * Iris->current_stats.Mana) / Iris->default_stats.Mana;
			life = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 150.0f, 79.0f, { 0, 58, life_x, 29 });
			mana = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 150.0f, 110.0f, { 0, 86, mana_x, 29 });
			portrait_1 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 12.0f, { 0, 115, 256, 128 });
		}

		if (!portraits_created)
		{
			iris_port = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 30.0f, 13.0f, { 124, 0, 64, 67 });
			portrait_1 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 12.0f, { 0, 115, 256, 128 });
			portrait_2 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, screen_height - 168, { 0, 115, 256, 128 });
			portrait_3 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, screen_width - 280, 12.0f, { 0, 115, 256, 128 });
			portrait_4 = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, screen_width - 280, screen_height - 168, { 0, 115, 256, 128 });
			//action_menu = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, 20.0f, 145.0f, { 0, 243, 256, 101 });
			portraits_created = true;
			action_menu_created = true;
		}

		// Camera Movment in map
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			App->render->camera.y += 4;
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			App->render->camera.x += 4;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			App->render->camera.y -= 4;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			App->render->camera.x -= 4;


		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		{
			ret = false;
		}
		break;
	default:
		break;
	}
	
	App->map->Draw();
	
	return ret;
}

// Called before all Updates
bool Scene::PostUpdate()
{
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	App->textures->UnLoad(main_menu_background);
	App->textures->UnLoad(options_background);
	App->textures->UnLoad(credits_page);
	return true;
}


// Load
bool Scene::Load(pugi::xml_node& savegame)
{
	return true;
}

//Save
bool Scene::Save(pugi::xml_node& _data) const
{
	return true;
}

void Scene::CreateMainMenu()
{
	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - ((button_margin * 2) + (button.h * 2) + (button.h /2)), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(new_game_button);
	load_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), new_game_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(load_game_button);
	options_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), load_game_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(options_button);
	credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), options_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(credits_button);
	exit_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), credits_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(exit_button);
	cherry_glasses_logo_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2) + 360, exit_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(cherry_glasses_logo_button);

	if (language)
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2) , new_game_button->position.second + (button.h / 2), "NEW GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2) , load_game_button->position.second + (button.h / 2), "LOAD GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2) , options_button->position.second + (button.h / 2), "OPTIONS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2) , credits_button->position.second + (button.h / 2), "CREDITS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2) , exit_button->position.second + (button.h / 2), "EXIT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2) , new_game_button->position.second + (button.h / 2), "NUEVA PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2) , load_game_button->position.second + (button.h / 2), "CARGAR PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2) , options_button->position.second + (button.h / 2), "OPCIONES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2) , credits_button->position.second + (button.h / 2), "CREDITOS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2) , exit_button->position.second + (button.h / 2), "SALIR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, cherry_glasses_logo_button->position.first + (button.w / 2) - (cherry_glasses_logo.w / 2) , cherry_glasses_logo_button->position.second + (button.h / 2) - (cherry_glasses_logo.h / 2), { 0, 0, 102, 58 });
	new_game_button->Select(SELECTED);
}

void Scene::CreateMMOptions()
{
	english_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - (option_background.h / 2) + (button.h * 2), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(english_button);
	spanish_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), english_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(spanish_button);
	volume_up_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), spanish_button->position.second + (button.h * 3), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_up_button);
	volume_down_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), volume_up_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_down_button);
	back_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_button);

	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_up_button->position.first + (button.w / 2), volume_up_button->position.second + (button.h / 2), "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_down_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_down_button->position.first + (button.w / 2), volume_down_button->position.second + (button.h / 2), "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	if (language)
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "ENGLISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "SPANISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), english_button->position.second - (button.h / 2), "CHOOSE LANGUAGE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), volume_up_button->position.second - (button.h / 2), "ADJUST VOLUME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "INGL�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "ESPA�OL", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_button->position.first + (button.w / 2), back_button->position.second + (button.h / 2), "ATR�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f - (button.w / 2) + 19, button.h - 65, "ESCOJER LENGUA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f - (button.w / 2) + 9, button.h + 170, "AJUSTAR VOLUMEN", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	english_button->Select(SELECTED);
}

void Scene::DeleteMenu()
{
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();
	main_menu_created = false;
}

void Scene::Navigate()
{
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN)
	{
		NavigateDown();
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN)
	{
		NavigateUp();
	}
}
void Scene::NavigateDown() 
{
	for (std::list<GUIButton*>::const_iterator it_vector = buttons.begin(); it_vector != buttons.end(); ++it_vector)
	{
		if ((*it_vector)->current_state == SELECTED) {
			if ((*it_vector) != buttons.back()) {
				(*it_vector)->Select(NORMAL);
				it_vector++;
				(*it_vector)->Select(SELECTED);
				break;
			}
			else
			{
				(*it_vector)->Select(NORMAL);
				it_vector = buttons.begin();
				(*it_vector)->Select(SELECTED);
			}
		}
	}
}

void Scene::NavigateUp() 
{
	for (std::list<GUIButton*>::const_iterator it_vector = buttons.begin(); it_vector != buttons.end(); ++it_vector)
	{
		if ((*it_vector)->current_state == SELECTED) {
			if ((*it_vector) != buttons.front()) {
				(*it_vector)->Select(NORMAL);
				it_vector--;
				(*it_vector)->Select(SELECTED);

				break;
			}
			else
			{
				(*it_vector)->Select(NORMAL);
				it_vector = buttons.end();
				it_vector--;
				(*it_vector)->Select(SELECTED);
			}
		}
	}
}


