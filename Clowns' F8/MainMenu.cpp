#include "Application.h"
#include "MainMenu.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTransitionManager.h"



MainMenu::MainMenu(SCENE_TYPE _type, pugi::xml_node& _config) : Scene(_type, _config)
{
	
}

// Destructor
MainMenu::~MainMenu()
{}


// Called before the first frame
bool MainMenu::Start()
{
	main_menu_background = App->textures->Load("Assets/Sprites/UI/main_menu_background.png");
	option_menu_background = App->textures->Load("Assets/Sprites/UI/option_menu_background.png");
	credits_menu_background = App->textures->Load("Assets/Sprites/UI/credits_menu_background.png");

	screen_width = App->window->GetScreenWidth();
	screen_height = App->window->GetScreenHeight();

	App->audio->LoadFx(press_fx_name.c_str());
	return true;
}

// Called before all Updates
bool MainMenu::PreUpdate()
{
	if (!App->scene_manager->music_created) {
		CreateMusic();
		CreateMainMenu();
	}

	return true;
}

// Called each loop iteration
bool MainMenu::Update(float _dt)
{
	bool ret = true;

	if (main_menu_created) 
	{
		if (new_game_button->has_been_clicked)
		{
			App->scene_manager->music_created = false;
			App->transition_manager->CreateFadeTransition(2, true, FIRST_BATTLE, Scene_1_color);

		}
		else if (load_game_button->has_been_clicked)
		{
		}
		else if (options_button->has_been_clicked)
		{
			Delete();
			CreateOptionMenu();
		}
		else if (credits_button->has_been_clicked)
		{
			Delete();
			CreateCreditsMenu();
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
	}
	else if (option_menu_created) 
	{
		ControlLanguageAndMusic();

		if (back_option_button->has_been_clicked || App->input->Decline())
		{
			Delete();
			CreateMainMenu();
		}
		else if (controls_button->has_been_clicked)
		{
			Delete();
			CreateControlsMenu();
		}
	}
	else if (controls_menu_created)
	{
		if (!controls_menu_created) {
			CreateControlsMenu();
		}
		if (accept_button->has_been_clicked) {

		}
		if (decline_button->has_been_clicked) {

		}
		if (character_stats_button->has_been_clicked) {

		}
		if (characeter_abilities_button->has_been_clicked) {

		}
		if (abilities_button->has_been_clicked) {

		}
		if (start_button->has_been_clicked) {

		}
		if (select_button->has_been_clicked) {

		}

		else if (back_controls_button->has_been_clicked || App->input->Decline())
		{
			Delete();
			CreateOptionMenu();
		}
	}
	else if (credits_menu_created)
	{
		if (back_credits_button->has_been_clicked || App->input->Decline())
		{
			Delete();
			CreateMainMenu();
		}
	}
	
	Navigate();

	return ret;
}

// Called before all Updates
bool MainMenu::PostUpdate()
{
	App->render->Blit(main_menu_background, 0, 0);

	if (option_menu_created || controls_menu_created)
	{
		App->render->Blit(option_menu_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	}
	else if(credits_menu_created)
	{
		App->render->Blit(credits_menu_background, (screen_width / 2) - (option_background.w / 2), (screen_height / 2) - (option_background.h / 2));
	}

	return true;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	App->textures->UnLoad(main_menu_background);
	App->textures->UnLoad(option_menu_background);
	App->textures->UnLoad(credits_menu_background);
	Delete();
	return true;
}


// Load
bool MainMenu::Load(pugi::xml_node& _data)
{
	return true;
}

//Save
bool MainMenu::Save(pugi::xml_node& _data) const
{
	return true;
}


void MainMenu::CreateMusic()
{
	App->scene_manager->music_created = true;
	App->audio->PlayMusic("Main_menu_8_bits.ogg");
}

void MainMenu::CreateMainMenu()
{
	main_menu_created = true;
	new_game_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - ((button_margin * 2) + (button.h * 2) + (button.h / 2)), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
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
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2), new_game_button->position.second + (button.h / 2), "NEW GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2), load_game_button->position.second + (button.h / 2), "LOAD GAME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2), options_button->position.second + (button.h / 2), "OPTIONS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2), credits_button->position.second + (button.h / 2), "CREDITS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2), exit_button->position.second + (button.h / 2), "EXIT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		new_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, new_game_button->position.first + (button.w / 2), new_game_button->position.second + (button.h / 2), "NUEVA PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		load_game_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, load_game_button->position.first + (button.w / 2), load_game_button->position.second + (button.h / 2), "CARGAR PARTIDA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		options_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, options_button->position.first + (button.w / 2), options_button->position.second + (button.h / 2), "OPCIONES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, credits_button->position.first + (button.w / 2), credits_button->position.second + (button.h / 2), "CR�DITOS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		exit_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, exit_button->position.first + (button.w / 2), exit_button->position.second + (button.h / 2), "SALIR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	cherry_glasses_logo_image = (GUIImage*)App->gui_manager->CreateGUIImage(GUI_ELEMENT_TYPE::GUI_IMAGE, cherry_glasses_logo_button->position.first + (button.w / 2) - (cherry_glasses_logo.w / 2), cherry_glasses_logo_button->position.second + (button.h / 2) - (cherry_glasses_logo.h / 2), { 0, 0, 102, 58 });
	new_game_button->Select(SELECTED);
}


void MainMenu::CreateOptionMenu()
{
	option_menu_created = true;
	english_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - (option_background.h / 2) + (button.h * 1.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(english_button);
	spanish_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), english_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(spanish_button);
	volume_up_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), spanish_button->position.second + (button.h * 2.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_up_button);
	volume_down_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), volume_up_button->position.second + (button_margin + button.h), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(volume_down_button);
	controls_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 2.7, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(controls_button);
	back_option_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 1.5, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_option_button);

	volume_up_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_up_button->position.first + (button.w / 2), volume_up_button->position.second + (button.h / 2), "+", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	volume_down_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, volume_down_button->position.first + (button.w / 2), volume_down_button->position.second + (button.h / 2), "-", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	if (language)
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "ENGLISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "SPANISH", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_option_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_option_button->position.first + (button.w / 2), back_option_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, controls_button->position.first + (button.w / 2), controls_button->position.second + (button.h / 2), "CONTROLS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), english_button->position.second - (button.h / 2), "CHOOSE LANGUAGE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, (screen_width / 2), volume_up_button->position.second - (button.h / 2), "ADJUST VOLUME", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		english_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, english_button->position.first + (button.w / 2), english_button->position.second + (button.h / 2), "INGL�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		spanish_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, spanish_button->position.first + (button.w / 2), spanish_button->position.second + (button.h / 2), "ESPA�OL", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_option_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_option_button->position.first + (button.w / 2), back_option_button->position.second + (button.h / 2), "ATR�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, controls_button->position.first + (button.w / 2), controls_button->position.second + (button.h / 2), "CONTROLES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		language_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f, english_button->position.second - (button.h / 2), "ESCOJER LENGUA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		volume_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, screen_width * 0.5f, volume_up_button->position.second - (button.h / 2), "AJUSTAR VOLUMEN", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	english_button->Select(SELECTED);

}

void MainMenu::CreateControlsMenu()
{
	controls_menu_created = true;
	accept_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) - (option_background.h / 2) + (button.h * 0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(accept_button);
	decline_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), accept_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(decline_button);
	character_stats_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), decline_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(character_stats_button);
	characeter_abilities_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), character_stats_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(characeter_abilities_button);
	abilities_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), characeter_abilities_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(abilities_button);
	start_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), abilities_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(start_button);
	select_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), start_button->position.second + (button_margin + button.h*0.5), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(select_button);
	back_controls_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), select_button->position.second + (button_margin + button.h * 1), { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_controls_button);

	if (language)
	{
		accept_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button->position.first + (button.w / 2), accept_button->position.second + (button.h / 2), "ACCEPT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		decline_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button->position.first + (button.w / 2), decline_button->position.second + (button.h / 2), "DECLINE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_stats_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, character_stats_button->position.first + (button.w / 2), character_stats_button->position.second + (button.h / 2), "CHARACTER STATS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_abilites_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, characeter_abilities_button->position.first + (button.w / 2), characeter_abilities_button->position.second + (button.h / 2), "CHARACTER ABILITIES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		abilities_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, abilities_button->position.first + (button.w / 2), abilities_button->position.second + (button.h / 2), "ABILITIES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		start_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button->position.first + (button.w / 2), start_button->position.second + (button.h / 2), "PAUSE", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		select_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, select_button->position.first + (button.w / 2), select_button->position.second + (button.h / 2), "SELECT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_controls_button->position.first + (button.w / 2), back_controls_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	}
	else
	{
		accept_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, accept_button->position.first + (button.w / 2), accept_button->position.second + (button.h / 2), "ACEPTAR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		decline_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, decline_button->position.first + (button.w / 2), decline_button->position.second + (button.h / 2), "DECLINAR", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_stats_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, character_stats_button->position.first + (button.w / 2), character_stats_button->position.second + (button.h / 2), "STATS PERSONAJES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		character_abilites_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, characeter_abilities_button->position.first + (button.w / 2), characeter_abilities_button->position.second + (button.h / 2), "HAB PERSONAJES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		abilities_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, abilities_button->position.first + (button.w / 2), abilities_button->position.second + (button.h / 2), "HABILIDADES", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		start_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, start_button->position.first + (button.w / 2), start_button->position.second + (button.h / 2), "PAUSA", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		select_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, select_button->position.first + (button.w / 2), select_button->position.second + (button.h / 2), "SELECT", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
		back_controls_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_controls_button->position.first + (button.w / 2), back_controls_button->position.second + (button.h / 2), "ATRAS", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);

	}
	accept_button->Select(SELECTED);
}

void MainMenu::CreateCreditsMenu()
{
	credits_menu_created = true;
	back_credits_button = (GUIButton*)App->gui_manager->CreateGUIButton(GUI_ELEMENT_TYPE::GUI_BUTTON, (screen_width / 2) - (button.w / 2), (screen_height / 2) + (option_background.h / 2) - button.h * 2, { 0, 0, 288, 64 }, { 0, 64, 288, 64 }, { 0, 128, 288, 64 });
	buttons.push_back(back_credits_button);
	back_credits_button->Select(SELECTED);
	if (language)
	{
		back_credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_credits_button->position.first + (button.w / 2), back_credits_button->position.second + (button.h / 2), "BACK", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
	else
	{
		back_credits_label = (GUILabel*)App->gui_manager->CreateGUILabel(GUI_ELEMENT_TYPE::GUI_LABEL, back_credits_button->position.first + (button.w / 2), back_credits_button->position.second + (button.h / 2), "ATR�S", { 0, 0, 0, 255 }, App->gui_manager->default_font_used);
	}
}

void MainMenu::Delete()
{
	App->gui_manager->DeleteAllGUIElements();
	buttons.clear();

	main_menu_created = false;
	option_menu_created = false;
	controls_menu_created = false;
	credits_menu_created = false;
}


void MainMenu::ControlLanguageAndMusic()
{
	if (english_button->has_been_clicked)
	{
		if (!language)
		{
			language = true;
			Delete();
			CreateOptionMenu();
		}
		english_button->Select(SELECTED);
	}
	else if (spanish_button->has_been_clicked)
	{
		if (language)
		{
			language = false;
			Delete();
			CreateOptionMenu();
			spanish_button->Select(NORMAL);
		}
		else spanish_button->Select(SELECTED);
	}

	if (volume_up_button->has_been_clicked)
	{
		App->audio->VolumeUp();
		volume_up_button->Select(SELECTED);
	}
	else if (volume_down_button->has_been_clicked)
	{
		App->audio->VolumeDown();
		volume_down_button->Select(SELECTED);
	}
}

