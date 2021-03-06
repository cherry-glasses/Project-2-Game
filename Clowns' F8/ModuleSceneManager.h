#ifndef __ModuleSceneManager_H__
#define __ModuleSceneManager_H__

#include "Module.h"
#include "Language.h"

class Scene;
struct SDL_Texture;
enum SCENE_TYPE;


enum TUTORIAL {
	GRETTINGS,
	MOVE,
	DEFENS,
	ATTACK,
	OFF
};

class ModuleSceneManager :	public Module
{
public:
	ModuleSceneManager();
	~ModuleSceneManager();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);
	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PreUpdate();
	// Called each loop iteration
	virtual bool Update(float dt);
	// Called each loop iteration
	virtual bool PostUpdate(float dt);
	// Called before quitting
	virtual bool CleanUp();
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	void ChangeScene(SCENE_TYPE _type);
	Scene* current_scene = nullptr;

	bool changing = false;
	bool loading = false;
	bool battle1_passed = false;
	bool battle2_passed = false;
	bool battle3_passed = false;

	Language* language = nullptr;
	SDL_Texture* objects_texture = nullptr;
	
	bool victory = false;

	bool start_new_game = true;
	bool start_fourth_game = true;

	bool tutorial_block = true;
	bool tutorial_block_actions = true;
	bool trailer = true;
	bool i_want_a_trailer = true;
	bool tutorial_hype = false;

	// Audios
	int accept_sfx;
	int decline_sfx;
	int	move_sfx;
	int	defend_sfx;
	int	critic_sfx;
	int	levelup_sfx;

	TUTORIAL logic = TUTORIAL::GRETTINGS;

private:
	pugi::xml_node scene_configs;

};

#endif // !__ModuleSceneManager_H_