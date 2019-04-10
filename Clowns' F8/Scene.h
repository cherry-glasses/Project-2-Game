#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class GUIImage;
class GUIButton;
class GUILabel;
class CharacterIris;
class CharacterSapphire;
struct SDL_Texture;

class Scene : public Module {


public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load
	bool Load(pugi::xml_node& savegame);

	//Save
	bool Save(pugi::xml_node& data) const;

private:
	void CreateMainMenu();
	void DeleteMenu();
	void CreateMMOptions();

	void Navigate();
	void NavigateDown();
	void NavigateUp();


private:
	SDL_Texture* main_menu_background = nullptr;
	SDL_Texture* options_background = nullptr;
	SDL_Texture* credits_page = nullptr;

	enum SCENES { MAIN_MENU, GLOBAL_MAP, MM_OPTIONS, MM_CREDITS, FIRST_BATTLE, SECOND_BATTLE, THIRD_BATTLE, FOURTH_BATTLE };


	SCENES current_scene = MAIN_MENU;

	SDL_Rect button;
	SDL_Rect cherry_glasses_logo;
	SDL_Rect option_background;
	int button_margin;
	int screen_width, screen_height;
	/*int button_half_width, button_half_height;
	int first_button_height, options_first_height;*/
	bool main_menu_created = false;
	bool mm_options_created = false;
	bool mm_credits_created = false;
	bool map_loaded = false;
	bool portraits_created = false;
	bool action_menu_created = false;
	//Main menu UI
	GUIButton* cherry_glasses_logo_button = nullptr;

	GUIButton* new_game_button = nullptr;
	GUIButton* load_game_button = nullptr;
	GUIButton* options_button = nullptr;
	GUIButton* credits_button = nullptr;
	GUIButton* exit_button = nullptr;

	GUIImage* cherry_glasses_logo_image = nullptr;
	GUILabel* new_game_label = nullptr;
	GUILabel* load_game_label = nullptr;
	GUILabel* options_label = nullptr;
	GUILabel* credits_label = nullptr;
	GUILabel* exit_label = nullptr;

	//Main menu options UI
	GUIButton* english_button = nullptr;
	GUIButton* spanish_button = nullptr;
	GUIButton* volume_up_button = nullptr;
	GUIButton* volume_down_button = nullptr;
	GUIButton* back_button = nullptr;
	GUILabel* english_label = nullptr;
	GUILabel* spanish_label = nullptr;
	GUILabel* volume_up_label = nullptr;
	GUILabel* volume_down_label = nullptr;
	GUILabel* back_label = nullptr;
	GUILabel* language_label = nullptr;
	GUILabel* volume_label = nullptr;

	//GUIImage* action_menu = nullptr;
	GUIButton* attack_button = nullptr;
	GUIButton* ability_button = nullptr;
	GUIButton* defend_button = nullptr;
	GUIImage* life = nullptr;
	GUIImage* mana = nullptr;
	GUIImage* portrait_1 = nullptr;
	GUIImage* portrait_2 = nullptr;
	GUIImage* portrait_3 = nullptr;
	GUIImage* portrait_4 = nullptr;
	GUIImage* iris_port = nullptr;
	int life_x = 124;
	int mana_x = 124;
	bool waiting_for_input = false;
	std::list<GUIButton*> buttons;

public:
	bool language = true; //true = English; false = Spanish
	CharacterIris* Iris;
	CharacterSapphire* Sapphire;

};

#endif //_SCENE_H_