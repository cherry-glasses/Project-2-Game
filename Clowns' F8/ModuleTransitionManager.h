#ifndef __ModuleTransitionManager_H_
#define __ModuleTransitionManager_H_

//This is the module that Marc will do on his research?

#include "Module.h"
#include "Transition.h"
#include "Scene.h"
#include "Color.h"

class ModuleTransitionManager :	public Module
{
public:
	ModuleTransitionManager();
	~ModuleTransitionManager();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);
	// Called before the first frame
	virtual bool Start();
	// Called each loop iteration
	virtual bool PostUpdate(float _dt);
	// Called before quitting
	virtual bool CleanUp();

	//--------------------------------

	void CreateFadeTransition(float transition_time, bool is_scene_change = false, SCENE_TYPE scene_to_transition = SCENE_TYPE::NONE, Color color = Black);
	void CreateSquaresTransition(float transition_time, bool is_scene_change = false, SCENE_TYPE scene_to_transition = SCENE_TYPE::NONE, Color color = Black);

	void DestroyTransition(Transition* transition_to_destroy);

	bool transitioning = false;

private:
	std::list<Transition*> active_transitions;

	//Util variables for diverse transitions
	SDL_Rect screen;

};
#endif // !__ModuleTransitionManager_H__