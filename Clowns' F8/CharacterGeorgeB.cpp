#include "Application.h"
#include "CharacterGeorgeB.h"
#include "ModuleMap.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleEntityManager.h"


CharacterGeorgeB::CharacterGeorgeB(ENTITY_TYPE _type, pugi::xml_node _config) : Character(_type, _config)
{
	CurrentMovement(IDLE_RIGHT);
	current = current_animation->GetCurrentFrame();
}

CharacterGeorgeB::~CharacterGeorgeB() {

}

void CharacterGeorgeB::SearchWalk() {

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	int x = pos.first - current_stats.PMove;
	int y = pos.second - current_stats.PMove;
	for (int i = 0; i < ((current_stats.PMove * 2) + 1) * ((current_stats.PMove * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.PMove) {
			x = pos.first - current_stats.PMove;
			++y;
		}
		if (y > pos.second + current_stats.PMove) {
			y = pos.second - current_stats.PMove;
		}
	}

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first += 1;
		tmp.second += 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	for (int i = 1; i <= current_stats.PMove; i++)
	{
		tmp.first -= 1;
		tmp.second -= 1;
		inrange_mov_list.push_back(tmp);
	}

	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_MOVE;
}

void CharacterGeorgeB::Walk()
{
	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		if (i == Cap)
		{
			objective_position.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));
		}
		++i;
	}

	if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
		CurrentMovement(WALK_LEFT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
		CurrentMovement(WALK_RIGHT);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_FRONT);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_BACK);
	}
	else {
		current_turn = SELECT_ACTION;
	}

	if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
		if (current_movement == WALK_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == WALK_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == WALK_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == WALK_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
		current_turn = SELECT_ACTION;
	}
}

void CharacterGeorgeB::SearchAttack() {
	objective_position.clear();
	inrange_mov_list.clear();
	possible_mov_list.clear();
	possible_map.clear();
	Cap = -1;

	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	int x = pos.first - current_stats.RangeAtk;
	int y = pos.second - current_stats.RangeAtk;
	for (int i = 0; i < ((current_stats.RangeAtk * 2) + 1) * ((current_stats.RangeAtk * 2) + 1); i++)
	{
		possible_mov_list.push_back({ x, y });
		++x;
		if (x > pos.first + current_stats.RangeAtk) {
			x = pos.first - current_stats.RangeAtk;
			++y;
		}
		if (y > pos.second + current_stats.RangeAtk) {
			y = pos.second - current_stats.RangeAtk;
		}
	}

	std::pair<int, int> tmp;
	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first += 1;
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.second += 1;
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.first -= 1;
	inrange_mov_list.push_back(tmp);

	tmp = App->map->WorldToMap((int)position.first, (int)position.second);
	tmp.second -= 1;
	inrange_mov_list.push_back(tmp);


	tmp.first = NULL;
	tmp.second = NULL;

	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.first < b.first; });
	inrange_mov_list.sort([](const std::pair<int, int> & a, const std::pair<int, int> & b) { return a.second < b.second; });

	current_turn = Entity::SELECT_ATTACK;
}

void CharacterGeorgeB::Attack()
{
	objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	if ((position.first == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
		|| (position.first == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
			&& position.second < App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second))
	{
		CurrentMovement(ATTACK_LEFT_FRONT);
	}
	else if (position.first < App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
	{
		CurrentMovement(ATTACK_RIGHT_FRONT);
	}
	else if (position.first > App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
	{
		CurrentMovement(ATTACK_LEFT_BACK);
	}
	else if (position.first == App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).first
		&& position.second > App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second).second)
	{
		CurrentMovement(ATTACK_RIGHT_BACK);
	}
	else {
		CurrentMovement(ATTACK_LEFT_FRONT);
	}

}

void CharacterGeorgeB::CurrentMovement(MOVEMENT _movement) {

	switch (_movement)
	{
	case Entity::IDLE_LEFT:
		current_movement = IDLE_LEFT;
		current_animation = &idle_left;
		break;
	case Entity::IDLE_RIGHT:
		current_movement = IDLE_RIGHT;
		current_animation = &idle_right;
		break;
	case Entity::IDLE_FRONT:
		current_movement = IDLE_FRONT;
		current_animation = &idle_front;
		break;
	case Entity::IDLE_BACK:
		current_movement = IDLE_BACK;
		current_animation = &idle_back;
		break;
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_left;
		position.first -= 2;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		position.first += 2;
		break;
	case Entity::WALK_FRONT:
		current_movement = WALK_FRONT;
		current_animation = &walk_front;
		position.second++;
		break;
	case Entity::WALK_BACK:
		current_movement = WALK_BACK;
		current_animation = &walk_back;
		position.second--;
		break;
	case Entity::ATTACK_LEFT_FRONT:
		current_movement = ATTACK_LEFT_FRONT;
		current_animation = &attack_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &hability_1_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &hability_1_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &hability_1_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_BACK: // ME HE QUEDADO AQU�
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &hability_1_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &hability_2_left_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &hability_2_right_front;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &hability_2_left_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &hability_2_right_back;
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkS, ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE);
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_LEFT_FRONT:
		current_movement = DEFEND_LEFT_FRONT;
		current_animation = &defend_left_front;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		current_movement = DEFEND_RIGHT_FRONT;
		current_animation = &defend_right_front;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_LEFT_BACK:
		current_movement = DEFEND_LEFT_BACK;
		current_animation = &defend_left_back;
		current_turn = END_TURN;
		break;
	case Entity::DEFEND_RIGHT_BACK:
		current_movement = DEFEND_RIGHT_BACK;
		current_animation = &defend_right_back;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_FRONT:
		current_movement = DEAD_LEFT_FRONT;
		current_animation = &dead_left_front;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_RIGHT_FRONT:
		current_movement = DEAD_RIGHT_FRONT;
		current_animation = &dead_right_front;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_BACK:
		current_movement = DEAD_LEFT_BACK;
		current_animation = &dead_left_back;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_RIGHT_BACK:
		current_movement = DEAD_RIGHT_BACK;
		current_animation = &dead_right_back;
		current_state = DEATH;
		current_turn = END_TURN;
		break;
	default:
		break;
	}
}

void CharacterGeorgeB::InputSelectMove() {

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + (sqrt(possible_mov_list.size()) - 1)) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap += sqrt(possible_mov_list.size()) - 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) + (sqrt(possible_mov_list.size()) - 1);
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - (sqrt(possible_mov_list.size()) - 1)) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap -= sqrt(possible_mov_list.size()) - 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) - (sqrt(possible_mov_list.size()) - 1);
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + (sqrt(possible_mov_list.size()) + 1)) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap += sqrt(possible_mov_list.size()) + 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) + (sqrt(possible_mov_list.size()) + 1);
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - (sqrt(possible_mov_list.size()) + 1)) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							if (App->pathfinding->IsWalkable({ (*possible_mov_2).first , (*possible_mov_2).second })
								&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end()
								&& !App->pathfinding->IsUsed({ (*possible_mov_2).first , (*possible_mov_2).second }, this))
							{
								Cap -= sqrt(possible_mov_list.size()) + 1;
								i = possible_mov_list.size();
							}
							else {
								Cap = (possible_mov_list.size() / 2) - (sqrt(possible_mov_list.size()) + 1);
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
}

void CharacterGeorgeB::InputSelectAttack() {

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - 1) {
						if ((*possible_mov).first > (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= 1;
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + 1) {
						if ((*possible_mov).first < (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += 1;
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second < (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap += sqrt(possible_mov_list.size());
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second > (*possible_mov_2).second)
						{
							if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov_2)) != inrange_mov_list.end())
							{
								Cap -= sqrt(possible_mov_list.size());
								i = possible_mov_list.size();
							}
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
}

