#ifndef __Application_H__
#define __Application_H__

#include <list>
#include <vector>
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"

//Modules
typedef std::list<Module*> ModuleList;

class ModuleWindow;
class ModuleRender;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModuleFonts;
class ModuleMap;
class ModuleSceneManager;
class ModuleEntityManager;
class ModuleGUIManager;
class ModulePathfinding;
class ModuleTransitionManager;
class ModuleParticleSystem;
class ModuleVideo;



class Application
{
public:

	// Constructor
	Application(int _argc, char* _args[]);

	// Destructor
	virtual ~Application();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* _module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int _index) const;
	const char* GetTitle() const;

	void LoadGame();
	void SaveGame() const;
	

	//Load emitters config
	pugi::xml_node LoadEmitters(pugi::xml_document& psystem_file) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;
	void GetSaveGames(std::list<std::string>& _list_to_fill) const;


public:

	ModuleWindow* window = nullptr;
	ModuleRender* render = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleFonts* fonts = nullptr;
	ModuleMap* map = nullptr;
	ModuleSceneManager* scene_manager = nullptr;
	ModuleEntityManager* entity_manager = nullptr;
	ModuleGUIManager* gui_manager = nullptr;
	ModulePathfinding* pathfinding = nullptr;
	ModuleTransitionManager *transition_manager = nullptr;
	ModuleParticleSystem	*particle_system	= nullptr;
	ModuleVideo *video_player = nullptr;

	bool debug = false;


private:

	ModuleList			modules;
	float				dt;
	int					argc;
	char**				args;
	std::string			title;
	std::string			organization;
	float				frame_rate;
	std::string			load_game;
	std::string			save_game;
	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	bool				want_to_quit = false;

	int					aux_seconds = 0;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	uint64				frame_count = 0;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;


	pugi::xml_document	config_file;




};

// Global var made extern for Application ---
extern Application* App;

#endif // __Application_H__