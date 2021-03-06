#include "Application.h"
#include "CharacterStorm.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"
#include "ModuleSceneManager.h"


CharacterStorm::CharacterStorm(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT_FRONT);
	current = current_animation->GetCurrentFrame(1);
	std::pair<int, int> pos = { 12,12 };
	int x = 0;
	int y = 0;
	for (int i = 0; i < (25 * 25); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > 24) {
			x = 0;
			++y;
		}
		if (y > 24) {
			y = 0;
		}
	}

	attacks_names.Attack_name = App->scene_manager->language->attack_storm;
	attacks_names.Ability_1_name = App->scene_manager->language->ability1_storm;
	attacks_names.Ability_2_name = App->scene_manager->language->ability2_storm;
	attacks_names.Ability_3_name = App->scene_manager->language->ability3_storm;
}

CharacterStorm::~CharacterStorm() {

}

void CharacterStorm::SearchWalk() {

	//Vampire Count
	if (vampire && vampire_count < 3) {
		vampire_count++;
	}
	else {
		vampire_count = 0;
	}

	//SearchWalk
	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	bool cutmove = false;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			cutmove = true;
		}
		if (cutmove)
		{
			nomov_list.push_back(tmp);
		}
		else
		{
			inrange_mov_list.push_back(tmp);
		}
	}
	cutmove = false;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			cutmove = true;
		}
		if (cutmove)
		{
			nomov_list.push_back(tmp);
		}
		else
		{
			inrange_mov_list.push_back(tmp);
		}
	}
	cutmove = false;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.second += 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			cutmove = true;
		}
		if (cutmove)
		{
			nomov_list.push_back(tmp);
		}
		else
		{
			inrange_mov_list.push_back(tmp);
		}
	}
	cutmove = false;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.second -= 1;
		if (!App->pathfinding->IsWalkable({ tmp.first , tmp.second })
			|| App->pathfinding->IsUsed({ tmp.first , tmp.second }, this))
		{
			cutmove = true;
		}
		if (cutmove)
		{
			nomov_list.push_back(tmp);
		}
		else
		{
			inrange_mov_list.push_back(tmp);
		}
	}

	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;
}

void CharacterStorm::SearchAttack() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	//inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.first -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAtk; i++)
	{
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ATTACK;
}

void CharacterStorm::SearchAbility_1() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAbility_1, tiles_range_attk);
	
	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	//inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.first -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.RangeAbility_1; i++)
	{
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ABILITY_1;

}

void CharacterStorm::SearchAbility_2() {

	inrange_mov_list.push_back({ App->map->WorldToMap(position.first, position.second) });
	current_turn = Entity::SELECT_ABILITY_2;
}

void CharacterStorm::SearchAbility_3() {

	inrange_mov_list.push_back({ App->map->WorldToMap(position.first, position.second) });
	current_turn = Entity::SELECT_ABILITY_3;
}

void CharacterStorm::CurrentMovement(MOVEMENT _movement) {

	flipX = false;
	switch (_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		current_movement = IDLE_LEFT_FRONT;
		current_animation = &idle_right_front;
		flipX = true;
		break;
	case Entity::IDLE_RIGHT_FRONT:
		current_movement = IDLE_RIGHT_FRONT;
		current_animation = &idle_right_front;
		break;
	case Entity::IDLE_LEFT_BACK:
		current_movement = IDLE_LEFT_BACK;
		current_animation = &idle_right_back;
		flipX = true;
		break;
	case Entity::IDLE_RIGHT_BACK:
		current_movement = IDLE_RIGHT_BACK;
		current_animation = &idle_right_back;
		break;
	case Entity::WALK_LEFT_FRONT:
		current_movement = WALK_LEFT_FRONT;
		current_animation = &walk_right_front;
		flipX = true;
		position.first -= 4;
		position.second += 2;
		break;
	case Entity::WALK_RIGHT_FRONT:
		current_movement = WALK_RIGHT_FRONT;
		current_animation = &walk_right_front;
		position.first += 4;
		position.second += 2;
		break;
	case Entity::WALK_LEFT_BACK:
		current_movement = WALK_LEFT_BACK;
		current_animation = &walk_right_back;
		flipX = true;
		position.first -= 4;
		position.second -= 2;
		break;
	case Entity::WALK_RIGHT_BACK:
		current_movement = WALK_RIGHT_BACK;
		current_animation = &walk_right_back;
		position.first += 4;
		position.second -= 2;
		break;
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_right_front;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_right_back;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, current_stats.Attack + current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_right_front;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_right_back;
		flipX = true;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ABILITY_1_RIGHT_BACK:
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		if (current_animation->isDone()) {
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			finish_attack = true;
		}
		break;
	case Entity::ABILITY_2_RIGHT:
		current_movement = ABILITY_2_RIGHT;
		current_animation = &ability_2_right_front;
		if (current_animation->isDone()) {
			objective_position.clear();
			App->entity_manager->ThrowAttack(objective_position, 0, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::ABILITY_3_RIGHT:
		current_movement = ABILITY_3_RIGHT;
		current_animation = &ability_3_right_front;
		if (current_animation->isDone()) {
			objective_position.clear();
			App->entity_manager->ThrowAttack(objective_position, 1, ENTITY_TYPE::ENTITY_CHARACTER_STORM, false);
			current_turn = END_TURN;
		}
		break;
	case Entity::DEFEND_LEFT_FRONT:
		current_movement = DEFEND_LEFT_FRONT;
		current_animation = &defend_right_front;
		flipX = true;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		current_movement = DEFEND_RIGHT_FRONT;
		current_animation = &defend_right_front;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_LEFT_BACK:
		current_movement = DEFEND_LEFT_BACK;
		current_animation = &defend_right_back;
		flipX = true;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT_BACK:
		current_movement = DEFEND_RIGHT_BACK;
		current_animation = &defend_right_back;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_FRONT:
		current_movement = DEAD_LEFT_FRONT;
		current_animation = &dead_right_front;
		flipX = true;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT_FRONT:
		current_movement = DEAD_RIGHT_FRONT;
		current_animation = &dead_right_front;
		current_state = DEATH;
		break;
	case Entity::DEAD_LEFT_BACK:
		current_movement = DEAD_LEFT_BACK;
		current_animation = &dead_right_back;
		flipX = true;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT_BACK:
		current_movement = DEAD_RIGHT_BACK;
		current_animation = &dead_right_back;
		current_state = DEATH;
		break;
	default:
		break;
	}
}


