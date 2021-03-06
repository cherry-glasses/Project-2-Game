#include "Log.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "Enemy.h"
#include "Character.h"
#include "Object.h"
#include "CharacterSapphire.h"
#include "CherryBlackGlasses.h"
#include "CharacterIris.h"
#include "CharacterStorm.h"
#include "CharacterGeorgeB.h"
#include "Boneyman.h"
#include "Pinkking.h"
#include "Burgdog.h"
#include "Hotdog.h"
#include "Polarbear.h"
#include "Polarpath.h"
#include "ModulePathfinding.h"
#include "ModuleMap.h"
#include "Scene.h"
#include "Entity.h"
#include "ModuleParticleSystem.h"
#include "Emitter.h"
#include <random>


ModuleEntityManager::ModuleEntityManager() : Module()
{
	name = "entity_manager";
}

// Destructor
ModuleEntityManager::~ModuleEntityManager()
{}

// Called before render is available
bool ModuleEntityManager::Awake(pugi::xml_node & _config)
{
	LOG("Loading Entities");
	entity_configs = _config;
	return true;
}

// Called before the first frame
bool ModuleEntityManager::Start()
{
	return true;
}


// Called each loop iteration
bool ModuleEntityManager::PreUpdate()
{
	if (entities.size() > 1) 
	{
		OrderEntitiesByAgility();
		if (starting && !App->scene_manager->tutorial_block) 
		{
			Entity *entityfirst = entities.front();
			entityfirst->current_turn = Entity::TURN::SEARCH_MOVE;
			starting = false;
		}
	}
	for (std::list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		if ((*enemy)->current_state == Entity::STATE::DEATH) {
			if ((*enemy)->current_turn == Entity::TURN::SEARCH_MOVE || (*enemy)->current_turn == Entity::TURN::END_TURN)
			{
				if (*enemy != enemies.back())
				{
					++enemy;
					(*enemy)->current_turn = Entity::TURN::SEARCH_MOVE;
					--enemy;
				}
				else {
					enemies.front()->current_turn = Entity::TURN::SEARCH_MOVE;
				}
			}
			// Level up and clean enemy
			App->entity_manager->LevelUP((*enemy)->exp);
			App->entity_manager->DeleteEntity((*enemy));
			break;
		}
	}
	for (std::list<Entity*>::iterator object = objects.begin(); object != objects.end(); ++object)
	{
		if (((Object*)*object)->used == true) {
			App->entity_manager->DeleteEntity((*object));
			break;
		}
	}
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->stunned == true)
		{
			if (stun_fx == START_TURN * 2)
			{
				Emitter* emitter1 = App->particle_system->AddEmiter({ (*entity)->GetPosition().first + 16, (*entity)->GetPosition().second - 60 }, EmitterType::EMITTER_TYPE_EFFECTS);
				emitter1->SetTextureRect({ 341, 89, 11, 13 });
				emitter1->SetSize(15, 15);
			}
			else if (stun_fx == START_TURN * 4)
			{
				Emitter* emitter2 = App->particle_system->AddEmiter({ (*entity)->GetPosition().first + 48, (*entity)->GetPosition().second - 60 }, EmitterType::EMITTER_TYPE_EFFECTS);
				emitter2->SetTextureRect({ 341, 89, 11, 13 });
				emitter2->SetSize(15, 15);
				stun_fx = 0;
			}
			++stun_fx;
		}
		if ((*entity)->current_turn == Entity::TURN::END_TURN && !App->scene_manager->tutorial_block)
		{
			(*entity)->current_turn = Entity::TURN::NONE;
			(*entity)->critic = false;
			if (*entity != entities.back())
			{
				++entity;
				(*entity)->current_turn = Entity::TURN::SEARCH_MOVE;
				
				StartingTurn((*entity));
			}
			else {
				entities.front()->current_turn = Entity::TURN::SEARCH_MOVE;
				StartingTurn(entities.front());
			}
		}
		if ((*entity)->stunned == true && (*entity)->current_turn == Entity::TURN::SEARCH_MOVE) {
			(*entity)->current_turn = Entity::TURN::END_TURN;
			(*entity)->stunned = false;
		}

		// BearTrap
		if (std::find(enemies.begin(), enemies.end(), (*entity)) != enemies.end()) {
			for (std::list<Entity*>::iterator object = objects.begin(); object != objects.end(); ++object) {
				if ((*object)->GetType() == ENTITY_TYPE::ENTITY_OBJECT_BEARTRAP && (*object)->GetPosition() == (*entity)->GetPosition()) {
					for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
						if ((*character)->GetType() == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB) {
							(*entity)->current_stats.Hp -= 25 + (*character)->current_stats.AtkF - (*entity)->current_stats.DefF;
							ThrowParticleDamage({ (*entity)->GetPosition().first, (*entity)->GetPosition().second - (*entity)->current.h }, 25 + (*character)->current_stats.AtkF - (*entity)->current_stats.DefF);
							((Object*)*object)->used = true;
						}
					}
				}
			}
		}

		
		
		(*entity)->PreUpdate();
		
	}

	int w, h;
	uchar* dat = NULL;

	if (App->map->CreateWalkabilityMap(w, h, &dat))
		App->pathfinding->SetMap(w, h, dat);
	return true;
}

bool ModuleEntityManager::Update(float _dt)
{
	if (!paused) {
		for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
		{
			(*entity)->Update(_dt);
		}
	}
	
	return true;
}


bool ModuleEntityManager::PostUpdate(float _dt)
{
	if (!paused) {
		if (entities.size() > 1)
		{
			OrderEntitiesByPosition();
		}
		for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
		{
			if ((*entity)->current_stats.Hp > (*entity)->default_stats.Hp) {
				(*entity)->current_stats.Hp = (*entity)->default_stats.Hp;
			}
			else if ((*entity)->current_stats.Hp < 0) {
				(*entity)->current_stats.Hp = 0;
			}
			if ((*entity)->current_stats.Mana >(*entity)->default_stats.Mana) {
				(*entity)->current_stats.Mana = (*entity)->default_stats.Mana;
			}
			else if ((*entity)->current_stats.Mana < 0) {
				(*entity)->current_stats.Mana = 0;
			}
			(*entity)->PostUpdate(_dt);

		}

	}
	return true;
}

// Called before quitting
bool ModuleEntityManager::CleanUp()
{
	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		(*entity)->CleanUp();
	}
	entities.clear();
	characters.clear();
	enemies.clear();
	objects.clear();
	paused = false;
	starting = true;

	return true;
}

// Load Game State
bool ModuleEntityManager::Load(pugi::xml_node & _data)
{
	exp_sapphire = _data.child("sapphire").attribute("value").as_int();
	exp_iris = _data.child("iris").attribute("value").as_int();
	exp_storm = _data.child("storm").attribute("value").as_int();
	exp_georgeb = _data.child("georgeb").attribute("value").as_int();
	return true;
}

// Save Game State
bool ModuleEntityManager::Save(pugi::xml_node & _data) const
{
	_data.append_child("sapphire").append_attribute("value") = exp_sapphire;
	_data.append_child("iris").append_attribute("value") = exp_iris;
	_data.append_child("storm").append_attribute("value") = exp_storm;
	_data.append_child("georgeb").append_attribute("value") = exp_georgeb;
	return true;
}

bool CompareByAgility(Entity* first, Entity* second) {
	return (first->current_stats.Agi > second->current_stats.Agi);
}
bool CompareByPosition(Entity* first, Entity* second) {
	return (first->GetPosition().second < second->GetPosition().second);
}

void ModuleEntityManager::OrderEntitiesByAgility() {
	entities.sort(CompareByAgility);
	characters.sort(CompareByAgility);
	enemies.sort(CompareByAgility);
	objects.sort(CompareByAgility);
}
void ModuleEntityManager::OrderEntitiesByPosition() {
	entities.sort(CompareByPosition);
	characters.sort(CompareByPosition);
	enemies.sort(CompareByPosition);
	objects.sort(CompareByPosition);
}

std::pair<int, int> ModuleEntityManager::NearestCharacter(std::pair<int, int> myposition){
	std::pair<int, int> tmp;
	std::pair<int, int> tmp_allied;
	float tmp_result;
	float tmp_result_2 = 30000;
	std::pair<int, int> position = App->map->WorldToMap(myposition.first, myposition.second);

	for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
		tmp_allied = (*character)->GetPosition();
		tmp_allied = App->map->WorldToMap(tmp_allied.first, tmp_allied.second);
		tmp_result = sqrt(((tmp_allied.first - position.first)*(tmp_allied.first - position.first)) + ((tmp_allied.second - position.second)*(tmp_allied.second - position.second)));
		if (tmp_result < tmp_result_2 && (*character)->current_state ==  (*character)->ALIVE) {
			tmp = tmp_allied;
			tmp_result_2 = tmp_result;
		}
			
	}
	tmp = App->map->MapToWorld(tmp.first, tmp.second);
	return tmp;

}

std::pair<int, int> ModuleEntityManager::CharactersPrioritzationAttack(std::pair<int, int>* AttackRange, int AttackRangeint)
{

	Entity* Charrr = nullptr;

	for (int i = 0; i < AttackRangeint; i++) {
		std::pair<int, int> pos = App->map->MapToWorld(AttackRange[i].first, AttackRange[i].second);
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
			if (pos == (*character)->GetPosition()) {
				if (Charrr == nullptr) {
					Charrr = (*character);
				}
				else {
					if ((*Charrr).current_stats.DefF >= (*character)->current_stats.DefF && (*character)->current_stats.Hp > 0) {
						Charrr = (*character);
					}
				}
			}
			

		}

	}

	return (*Charrr).GetPosition();
}

std::pair<int, int> ModuleEntityManager::AiHeals(std::pair<int, int>* AttackRange, int AttackRangeint)
{
	Entity* Charrr = nullptr;

	std::pair<int, int> position;
	for (int i = 0; i < AttackRangeint; i++) {
		std::pair<int, int> pos = App->map->MapToWorld(AttackRange[i].first, AttackRange[i].second);
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
			if (pos == (*character)->GetPosition()) {
				if (Charrr == nullptr && (*character)->current_stats.Hp > 0) {
					Charrr = (*character);
				}
				else {
					if (Charrr != nullptr) {
						if ((*Charrr).current_stats.DefF >= (*character)->current_stats.DefF && (*character)->current_stats.Hp > 0) {
							Charrr = (*character);
						}
					}
					
				}
			}
		}

	}
	position = Charrr->GetPosition();

	for (int i = 0; i < AttackRangeint; i++) {
		std::pair<int, int> pos = App->map->MapToWorld(AttackRange[i].first, AttackRange[i].second);
		for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie) {
			if (pos == (*enemie)->GetPosition()) {

				if ((*enemie)->current_stats.Hp <= ((*enemie)->default_stats.Hp / 3)) {
					position = (*enemie)->GetPosition();
					break;
				}

			}


		}


	}

	return position;
}

Entity* ModuleEntityManager::CreateEntity(ENTITY_TYPE _type)
{
	Entity* tmp = nullptr;

	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
		tmp = new CharacterSapphire(_type, entity_configs.child("sapphire"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		tmp = new CharacterIris(_type, entity_configs.child("iris"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
		tmp = new CharacterStorm(_type, entity_configs.child("storm"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
		tmp = new CharacterGeorgeB(_type, entity_configs.child("georgeb"));
		entities.push_back(tmp);
		characters.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN:
		for (int i = 0; i < 4; i++)
		{
			tmp = new Boneyman(_type, entity_configs.child("boneyman"), i);
			entities.push_back(tmp);
			enemies.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_PINKKING:
		tmp = new Pinkking(_type, entity_configs.child("pinkking"));
		entities.push_back(tmp);
		enemies.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
		for (int i = 0; i < 4; i++)
		{
			tmp = new Burgdog(_type, entity_configs.child("burgdog"), i);
			entities.push_back(tmp);
			enemies.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
		tmp = new Hotdog(_type, entity_configs.child("hotdog"));
		entities.push_back(tmp);
		enemies.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_POLARBEAR:
		for (int i = 0; i < 4; i++)
		{
			tmp = new Polarbear(_type, entity_configs.child("polarbear"), i);
			entities.push_back(tmp);
			enemies.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_POLARPATH:
		tmp = new Polarpath(_type, entity_configs.child("polarpath"));
		entities.push_back(tmp);
		enemies.push_back(tmp);
		break;

	case ENTITY_TYPE::ENTITY_ENEMY_CHERRYBLACKGLASSES:
		tmp = new CherryBlackGlasses(_type, entity_configs.child("cherryblackglasses")); //hay que cambiar esto
		entities.push_back(tmp);
		enemies.push_back(tmp);
		break;

	case ENTITY_TYPE::ENTITY_OBJECT_TREE1:
		for (int i = 0; i < 5; i++)
		{
			tmp = new Object(_type, entity_configs.child("tree1"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_TREE2:
		for (int i = 0; i < 7; i++)
		{
			tmp = new Object(_type, entity_configs.child("tree2"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_TREE3:
		for (int i = 0; i < 15; i++)
		{
			tmp = new Object(_type, entity_configs.child("tree3"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_STONE:
		for (int i = 0; i < 13; i++)
		{
			tmp = new Object(_type, entity_configs.child("stone"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_STONE2:
		for (int i = 0; i < 10; i++)
		{
			tmp = new Object(_type, entity_configs.child("stone_2"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_STONE3:
		for (int i = 0; i < 6; i++)
		{
			tmp = new Object(_type, entity_configs.child("stone_3"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_STALAGMITE:
		for (int i = 0; i < 9; i++)
		{
			tmp = new Object(_type, entity_configs.child("stalagmite"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_VOLCANO:
		tmp = new Object(_type, entity_configs.child("volcano"), 0);
		entities.push_back(tmp);
		objects.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_ICE:
		for (int i = 0; i < 17; i++)
		{
			tmp = new Object(_type, entity_configs.child("ice"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_MOUNTAIN:
		tmp = new Object(_type, entity_configs.child("mountain"), 0);
		tmp->position_margin.second = 0;
		entities.push_back(tmp);
		objects.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_MOUNTAIN2:
		tmp = new Object(_type, entity_configs.child("mountain_2"), 0);
		tmp->position_margin.second = 0;
		entities.push_back(tmp);
		objects.push_back(tmp);
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_BLOODSTREAM:
		for (int i = 0; i < 2; i++)
		{
			tmp = new Object(_type, entity_configs.child("bloodstream"), i);
			entities.push_back(tmp);
			objects.push_back(tmp);
		}
		break;
	case ENTITY_TYPE::ENTITY_OBJECT_BEARTRAP:
		tmp = new Object(_type, entity_configs.child("beartrap"), 0);
		entities.push_back(tmp);
		objects.push_back(tmp);
		break;
	default:
		break;
	}

	return tmp;
}

bool ModuleEntityManager::DeleteEntity(Entity * entity)
{
	if (std::find(enemies.begin(), enemies.end(), (entity)) != enemies.end()) {
		enemies.remove(entity);
	}
	else if(std::find(objects.begin(), objects.end(), (entity)) != objects.end()) {
		objects.remove(entity);
	}
	entities.remove(entity);
	entity->CleanUp();
	entity = nullptr;
	
	return true;
}



void ModuleEntityManager::ThrowAttack(std::vector<std::pair<int, int>> _positions, int _damage, ENTITY_TYPE _type, bool _special) 
{
	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
		if (_positions.empty()) { //Ability 3
			for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
			{
				if ((*character)->current_stats.Hp > 0)
				{
					(*character)->current_stats.Hp += _damage / 2;
					ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, -(_damage / 2));
				}
			}
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				if (0 < _damage - (*enemie)->current_stats.DefS)
				{
					(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefS;
					ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefS);
				}
				else
				{
					ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
				}
				(*enemie)->damaged = true;
			}
		}
		else { // Attack, Ability 1 and Ability 2
			for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*character)->GetPosition() == (*position) && (*character)->current_stats.Hp > 0)
					{
						(*character)->current_stats.Hp += _damage / 2;
						ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, -(_damage / 2));
					}
				}
			}
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*enemie)->GetPosition() == (*position))
					{
						if (_special)
						{
							if (0 < _damage - (*enemie)->current_stats.DefS)
							{
								(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefS;
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefS);
							}
							else
							{
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
							}
							(*enemie)->damaged = true;
						}
						else
						{
							if (0 < _damage - (*enemie)->current_stats.DefF)
							{
								(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefF;
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefF);
							}
							else
							{
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
							}
							(*enemie)->damaged = true;
						}

					}
				}
			}
		}
		
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
		if (_positions.empty()) { //Ability 2
			for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
			{
				if ((*character)->GetType() == ENTITY_TYPE::ENTITY_CHARACTER_IRIS) 
				{
					(*character)->current_stats.AtkF += _damage;
					(*character)->current_stats.DefF += _damage;
				}
			}
		}
		else { //Attack, Ability 1 and Ability 3
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*enemie)->GetPosition() == (*position))
					{
						if (0 < _damage - (*enemie)->current_stats.DefF)
						{
							(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefF;
							ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefF);
						}
						else
						{
							ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
						}
						(*enemie)->damaged = true;
					}
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
		if (_positions.empty()) {
			if (_damage == 0) // Ability 2
			{
				for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
				{
					if ((*character)->GetType() == ENTITY_TYPE::ENTITY_CHARACTER_STORM)
					{
						(*character)->invulnerable = true;
					}
				}
			}
			else // Ability 3
			{
				for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
				{
					if ((*character)->GetType() == ENTITY_TYPE::ENTITY_CHARACTER_STORM)
					{
						(*character)->vampire = true;
					}
				}
			}
		}
		else { 
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*enemie)->GetPosition() == (*position))
					{
						if (_damage == 0) // Ability 1
						{ 
							(*enemie)->stunned = true;
						}
						else 
						{ // Attack and Ability 3 Suck
							if (0 < _damage - (*enemie)->current_stats.DefF)
							{
								(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefF;
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefF);
							}
							else
							{
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
							}
							(*enemie)->damaged = true;

							for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
							{
								if ((*character)->GetType() == ENTITY_TYPE::ENTITY_CHARACTER_STORM && (*character)->vampire)
								{
									(*character)->current_stats.Hp += (_damage - (*enemie)->current_stats.DefF) / 2;
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, -((_damage - (*enemie)->current_stats.DefF) / 2));
								}
							}
						}
					}
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
		if (_damage == 0) { //Ability 1
			for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
			{
				Object *tmp = (Object*)CreateEntity(ENTITY_TYPE::ENTITY_OBJECT_BEARTRAP);
				(*tmp).SetPosition((*position).first, (*position).second);
			}
		}
		else { //Attack, Ability 2 and Ability 3
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*enemie)->GetPosition() == (*position))
					{
						if (_special)
						{
							if (0 < _damage - (*enemie)->current_stats.DefS)
							{
								(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefS;
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefS);
							}
							else
							{
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
							}
							(*enemie)->damaged = true;
						}
						else
						{
							if (0 < _damage - (*enemie)->current_stats.DefF)
							{
								(*enemie)->current_stats.Hp -= _damage - (*enemie)->current_stats.DefF;
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, _damage - (*enemie)->current_stats.DefF);
							}
							else
							{
								ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, 0);
							}
							(*enemie)->damaged = true;
						}
					}
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_PINKKING:
	case ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN:
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
	case ENTITY_TYPE::ENTITY_ENEMY_POLARPATH:
	case ENTITY_TYPE::ENTITY_ENEMY_POLARBEAR:
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
		{
			if (!(*character)->invulnerable)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*character)->GetPosition() == (*position))
					{
						if ((*character)->defend) {
							if (_special)
							{
								if (0 < _damage - ((*character)->current_stats.DefS * 1.25))
								{
									(*character)->current_stats.Hp -= _damage - ((*character)->current_stats.DefS * 1.25);
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefS * 1.25);
								}
								else
								{
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h },0);
								}
								
								(*character)->damaged = true;
							}
							else
							{
								if (0 < _damage - ((*character)->current_stats.DefF * 1.25))
								{
									(*character)->current_stats.Hp -= _damage - ((*character)->current_stats.DefF * 1.25);
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefF * 1.25);
								}
								else
								{
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
								}
								
								(*character)->damaged = true;
							}

						}
						else {
							if (_special)
							{
								if (0 < _damage - (*character)->current_stats.DefS)
								{
									(*character)->current_stats.Hp -= _damage - (*character)->current_stats.DefS;
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefS);
								}
								else
								{
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h },0);
								}
								(*character)->damaged = true;
							}
							else
							{
								if (0 < _damage - (*character)->current_stats.DefF)
								{
									(*character)->current_stats.Hp -= _damage - (*character)->current_stats.DefF;
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefF);
								}
								else
								{
									ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
								}
								(*character)->damaged = true;
								
							}
						}

					}
				}
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_CHERRYBLACKGLASSES:


		if (_positions.empty()) {
			for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
			{
				if (!(*character)->invulnerable)
				{
					
					if (_special)
					{
						
					if (0 < _damage - (*character)->current_stats.DefS)
					{
						(*character)->current_stats.Hp -= _damage - (*character)->current_stats.DefS;
						ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefS);
					}
					else
					{
						ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
					}

					(*character)->damaged = true;
					}
					else
					{
						if (0 < _damage - (*character)->current_stats.DefF)
						{
							(*character)->current_stats.Hp -= _damage - (*character)->current_stats.DefF;
							ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefF);
						}
						else
						{
							ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
						}

						(*character)->damaged = true;
					}
					
				}
			}
			for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie)
			{
				for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
				{
					if ((*enemie)->GetPosition() == (*position))
					{
						if (0 < _damage - (*enemie)->current_stats.DefF)

						{
							(*enemie)->current_stats.Hp += _damage / 2;
							ThrowParticleDamage({ (*enemie)->GetPosition().first, (*enemie)->GetPosition().second - (*enemie)->current.h }, -(_damage / 2));

						}

					}
				}
			}
		}
		else 
		{
			for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
			{
				if (!(*character)->invulnerable)
				{
					for (std::vector<std::pair<int, int>>::iterator position = _positions.begin(); position != _positions.end(); ++position)
					{
						if ((*character)->GetPosition() == (*position))
						{
							if ((*character)->defend) {
								if (_special)
								{
									if (0 < _damage - ((*character)->current_stats.DefS * 1.25))
									{
										(*character)->current_stats.Hp -= _damage - ((*character)->current_stats.DefS * 1.25);
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefS * 1.25);
									}
									else
									{
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
									}

									(*character)->damaged = true;
								}
								else
								{
									if (0 < _damage - ((*character)->current_stats.DefF * 1.25))
									{
										(*character)->current_stats.Hp -= _damage - ((*character)->current_stats.DefF * 1.25);
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefF * 1.25);
									}
									else
									{
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
									}

									(*character)->damaged = true;
								}

							}
							else {
								if (_special)
								{
									if (0 < _damage - (*character)->current_stats.DefS)
									{
										(*character)->current_stats.Hp -= _damage - (*character)->current_stats.DefS;
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefS);
									}
									else
									{
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
									}
									(*character)->damaged = true;
								}
								else
								{
									if (0 < _damage - (*character)->current_stats.DefF)
									{
										(*character)->current_stats.Hp -= _damage - (*character)->current_stats.DefF;
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, _damage - (*character)->current_stats.DefF);
									}
									else
									{
										ThrowParticleDamage({ (*character)->GetPosition().first, (*character)->GetPosition().second - (*character)->current.h }, 0);
									}
									(*character)->damaged = true;
								}
							}

						}
					}
				}
			}

		}
		
		

		break;
	case ENTITY_TYPE::NO_TYPE:
		break;
	default:
		break;
	}
	
}

void ModuleEntityManager::ThrowParticleDamage(std::pair<int,int> _pos,int _damage)
{
	Emitter* emitter = nullptr;
	Emitter* effects1 = nullptr;
	Emitter* effects2 = nullptr;
	Emitter* effects3 = nullptr;
	Emitter* effects4 = nullptr;
	Emitter* effects5 = nullptr;
	Emitter* effects6 = nullptr;
	Emitter* effects7 = nullptr;
	Emitter* effects8 = nullptr;
	effects1 = App->particle_system->AddEmiter({ _pos.first, _pos.second + 52 }, EMITTER_TYPE_EFFECTS);
	effects2 = App->particle_system->AddEmiter({ _pos.first, _pos.second + 84 }, EMITTER_TYPE_EFFECTS);
	effects3 = App->particle_system->AddEmiter({ _pos.first + 15, _pos.second + 36 }, EMITTER_TYPE_EFFECTS);
	effects4 = App->particle_system->AddEmiter({ _pos.first + 15, _pos.second + 68 }, EMITTER_TYPE_EFFECTS);
	effects5 = App->particle_system->AddEmiter({ _pos.first + 30, _pos.second + 52 }, EMITTER_TYPE_EFFECTS);
	effects6 = App->particle_system->AddEmiter({ _pos.first + 30, _pos.second + 84 }, EMITTER_TYPE_EFFECTS);
	effects7 = App->particle_system->AddEmiter({ _pos.first + 45, _pos.second + 36 }, EMITTER_TYPE_EFFECTS);
	effects8 = App->particle_system->AddEmiter({ _pos.first + 45, _pos.second + 58 }, EMITTER_TYPE_EFFECTS);
	int num = 0;
	bool healing = false;
	if (_damage < 0)
	{
		_damage = sqrt(_damage * _damage);
		healing = true;
	}

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			num = _damage / 100;
			break;
		case 1:
			num = (_damage % 100) / 10;
			break;
		case 2:
			num = (_damage % 100) % 10;
			break;
		default:
			break;
		}
		switch (num)
		{
		case 0:
			if ((i == 1 && _damage > 99) || i == 2 )
			{
				emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
				emitter->SetTextureRect({ 256,64,64,64 });
			}
			break;
		case 1:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({0,0,64,64});
			break;
		case 2:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 64,0,64,64 });
			break;
		case 3:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 128,0,64,64 });
			break;
		case 4:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 192,0,64,64 });
			break;
		case 5:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 256,0,64,64 });
			break;
		case 6:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 0,64,64,64 });
			break;
		case 7:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 64,64,64,64 });
			break;
		case 8:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 128,64,64,64 });
			break;
		case 9:
			emitter = App->particle_system->AddEmiter(_pos, EMITTER_TYPE_NUM);
			emitter->SetTextureRect({ 192,64,64,64 });
			break;
		default:
			break;
		}

		switch (i)
		{
		case 0:
			break;
		case 1:
			if(emitter != nullptr)
				emitter->MoveEmitter({ _pos.first + (64/2), _pos.second });
			break;
		case 2:
			if (emitter != nullptr)
				emitter->MoveEmitter({ _pos.first + (128/2), _pos.second });
			break;
		default:
			break;
		}

		if (healing && emitter != nullptr)
		{
			emitter->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects1->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects2->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects3->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects4->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects5->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects6->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects7->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
			effects8->SetColor({ 0, 200, 0, 255 }, { 0, 200, 0, 50 });
		}
			
		
	}

	
}

bool ModuleEntityManager::UpdateWalk(std::pair<int, int> tile_id) {
	bool ret = false;
	std::pair<int, int> tmp;
	for (std::list<Entity*>::iterator enemie = enemies.begin(); enemie != enemies.end(); ++enemie) {
		tmp = (*enemie)->GetPosition();
		tmp = App->map->WorldToMap(tmp.first, tmp.second);
		if (tile_id == tmp) {
			ret = true;
		}
	}
	return ret;
}

bool ModuleEntityManager::ThereAreCharAlive()
{
	bool flag = false;
	for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
		if ((*character)->current_state == (*character)->ALIVE) {
			flag = true;
			break;
		}
	}
	return flag;
}

float ModuleEntityManager::CalculateDistance(std::pair<int, int> origin, std::pair<int, int> objective)
{
	float ret = sqrt((objective.first - origin.first)*(objective.first - origin.first) + ((objective.second - origin.second)*(objective.second - origin.second)));
	return ret;
}



std::pair<int, int>* ModuleEntityManager::RangeOfAttack(std::pair<int, int> myposition, int radius, int& size) {

	std::list<std::pair<int, int>> frontier;
	std::list<std::pair<int, int>> visited;
	bool flag = false;

	std::pair<int, int> aux;
	// calcular con los 2 radios y restar.
	frontier.push_back(myposition);
	//visited.push_back(myposition);
	while (frontier.size() != 0) {
		aux = frontier.front();
		frontier.pop_front();
		flag = false;
		for (std::list<std::pair<int, int>>::iterator eshorrible = visited.begin(); eshorrible != visited.end(); ++eshorrible) {
			if (aux == (*eshorrible)) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			for (int i = -1; i <= 1; i += 2) {
				for (int j = -1; j <= 1; j += 2) {
					std::pair<int, int> tmp = aux;
					if (i == -1)
						tmp.first += j;
					else 
						tmp.second += j;
					
					if (InRange(myposition, tmp, radius)) {
						frontier.push_back(tmp);
					}
				}
			}
			visited.push_back(aux);
		}
	}

	size = visited.size();
	std::pair<int, int>* ret = new std::pair<int,int>[visited.size()];
	int i = 0;
	for (std::list<std::pair<int, int>>::iterator eshorrible = visited.begin(); eshorrible != visited.end(); ++eshorrible, i++) {
		ret[i] = (*eshorrible);
	}

	return ret;
}

void ModuleEntityManager::StartingTurn(Entity* _entity)
{
	_entity->current_stats.Mana += 10;
	_entity->defend = false;
	_entity->invulnerable = false;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 100);
	if (distribution(generator) < _entity->current_stats.Crit)
	{
		_entity->critic = true;
	}
	else 
	{
		_entity->critic = false;
	}
}

void ModuleEntityManager::LevelUP(int _exp)
{
	if (App->scene_manager->current_scene->type == SCENE_TYPE::FOURTH_BATTLE)
	{
		if (App->scene_manager->start_fourth_game)
		{
			for (std::list<Entity*>::iterator enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
			{
				if ((*enemy)->GetType() != ENTITY_TYPE::ENTITY_ENEMY_CHERRYBLACKGLASSES)
				{
					(*enemy)->default_stats.Hp += (*enemy)->default_stats.Hp * 3;
					(*enemy)->default_stats.Mana += (*enemy)->default_stats.Mana;
					(*enemy)->default_stats.AtkF += (*enemy)->default_stats.AtkF;
					(*enemy)->default_stats.AtkS += (*enemy)->default_stats.AtkS;
					(*enemy)->default_stats.DefF += (*enemy)->default_stats.DefF;
					(*enemy)->default_stats.DefS += (*enemy)->default_stats.DefS;
					(*enemy)->default_stats.Crit += (*enemy)->default_stats.Crit;

					(*enemy)->current_stats = (*enemy)->default_stats;
				}
			}
			App->scene_manager->start_fourth_game = false;
		}
	}

	if (!App->scene_manager->start_new_game)
	{
		
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character)
		{
			if ((*character)->current_state == Entity::ALIVE)
			{

				switch ((*character)->GetType())
				{
				case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
					exp_sapphire += _exp;
					(*character)->exp = exp_sapphire;
					break;
				case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
					exp_iris += _exp;
					(*character)->exp = exp_iris;
					break;
				case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
					exp_storm += _exp;
					(*character)->exp = exp_storm;
					break;
				case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
					exp_georgeb += _exp;
					(*character)->exp = exp_georgeb;
					break;
				default:
					break;
				}

				while ((*character)->level < 10 && (*character)->levels.at((*character)->level - 1) <= (*character)->exp)
				{
					(*character)->default_stats.Hp += (*character)->evolution_stats.Hp * (*character)->level * 2;
					(*character)->default_stats.Mana += (*character)->evolution_stats.Mana * (*character)->level;
					(*character)->default_stats.AtkF += (*character)->evolution_stats.AtkF * (*character)->level;
					(*character)->default_stats.AtkS += (*character)->evolution_stats.AtkS * (*character)->level;
					(*character)->default_stats.DefF += (*character)->evolution_stats.DefF * (*character)->level;
					(*character)->default_stats.DefS += (*character)->evolution_stats.DefS * (*character)->level;
					(*character)->default_stats.Crit += (*character)->evolution_stats.Crit * (*character)->level;

					(*character)->current_stats = (*character)->default_stats;
					++(*character)->level;
					Emitter* emitter1 = App->particle_system->AddEmiter({ (*character)->GetPosition().first + 16, (*character)->GetPosition().second - (*character)->current.h }, EmitterType::EMITTER_TYPE_EFFECTS);
					emitter1->SetTextureRect({ 341, 105, 12, 13 });
					emitter1->SetSize(20, 20);
					Emitter* emitter2 = App->particle_system->AddEmiter({ (*character)->GetPosition().first + 48, (*character)->GetPosition().second - (*character)->current.h }, EmitterType::EMITTER_TYPE_EFFECTS);
					emitter2->SetTextureRect({ 341, 105, 12, 13 });
					emitter2->SetSize(20, 20);
				}

				while ((*character)->level > 1 && (*character)->levels.at((*character)->level - 2) > (*character)->exp)
				{
					--(*character)->level;
					(*character)->default_stats.Hp -= (*character)->evolution_stats.Hp * (*character)->level;
					(*character)->default_stats.Mana -= (*character)->evolution_stats.Mana * (*character)->level;
					(*character)->default_stats.AtkF -= (*character)->evolution_stats.AtkF * (*character)->level;
					(*character)->default_stats.AtkS -= (*character)->evolution_stats.AtkS * (*character)->level;
					(*character)->default_stats.DefF -= (*character)->evolution_stats.DefF * (*character)->level;
					(*character)->default_stats.DefS -= (*character)->evolution_stats.DefS * (*character)->level;
					(*character)->default_stats.Crit -= (*character)->evolution_stats.Crit * (*character)->level;

					(*character)->current_stats = (*character)->default_stats;
				}
			}
		}
	}
	App->scene_manager->start_new_game = false;
	
}

bool ModuleEntityManager::Characters_around(std::pair<int, int> position)
{
	bool flag = false;
	std::pair<int, int> Array[8];

	Array[0].first = position.first + 1;
	Array[0].second = position.second;

	Array[1].first = position.first +1;
	Array[1].second = position.second + 1;

	Array[3].first = position.first;
	Array[3].second = position.second +1 ;

	Array[4].first = position.first-1;
	Array[4].second = position.second+1;

	Array[4].first = position.first-1;
	Array[4].second = position.second;

	Array[5].first = position.first-1;
	Array[5].second = position.second-1;

	Array[6].first = position.first;
	Array[6].second = position.second-1;

	Array[7].first = position.first+1;
	Array[7].second = position.second-1;

	for (int i = 0; i < 8; i++) {
		Array[i] = App->map->MapToWorld(Array[i].first, Array[i].second);
		for (std::list<Entity*>::iterator character = characters.begin(); character != characters.end(); ++character) {
			std::pair<int, int> helper = (*character)->GetPosition();
			if (Array[i].first == helper.first && Array[i].second == helper.second && (*character)->current_stats.Hp > 0) {
				flag = true;
				break;

			}
		}
		if (flag)
			break;

	}

	return flag;
}
