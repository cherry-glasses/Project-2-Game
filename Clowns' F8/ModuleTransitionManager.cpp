#include "Log.h"
#include "Application.h"
#include "ModuleTransitionManager.h"
#include "Fade.h"
#include "Squares.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"


ModuleTransitionManager::ModuleTransitionManager()
{

}

ModuleTransitionManager::~ModuleTransitionManager()
{

}

bool ModuleTransitionManager::Awake(pugi::xml_node &)
{
	LOG("Awakening Transition Manager");
	return true;
}

bool ModuleTransitionManager::Start()
{
	LOG("Starting Transition Manager");

	return true;
}


bool ModuleTransitionManager::PostUpdate(float _dt)
{
	for (std::list<Transition*>::iterator transition = active_transitions.begin(); transition != active_transitions.end(); ++transition)
	{
		if ((*transition)->todestroy) {
			DestroyTransition((*transition));
			break;
		}
	}
	for (std::list<Transition*>::iterator transition = active_transitions.begin(); transition != active_transitions.end(); ++transition)
	{
		(*transition)->PostUpdate(_dt);
	}

	return true;
}

bool ModuleTransitionManager::CleanUp()
{
	active_transitions.clear();

	return true;
}

void ModuleTransitionManager::CreateFadeTransition(float transition_time, bool is_scene_change, SCENE_TYPE scene_to_transition, Color color)
{
	active_transitions.push_back(new Fade(transition_time, is_scene_change, scene_to_transition, color));
}

void ModuleTransitionManager::CreateSquaresTransition(float transition_time, bool is_scene_change, SCENE_TYPE scene_to_transition, Color color)
{
	active_transitions.push_back(new Squares(transition_time, is_scene_change, scene_to_transition, color));
}

void ModuleTransitionManager::DestroyTransition(Transition * transition_to_destroy)
{
	active_transitions.remove(transition_to_destroy);
	delete transition_to_destroy;
}