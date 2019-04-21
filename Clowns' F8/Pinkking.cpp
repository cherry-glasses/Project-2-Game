#include "Application.h"
#include "Pinkking.h"
#include "ModuleEntityManager.h"
#include "ModulePathfinding.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


Pinkking::Pinkking(ENTITY_TYPE _type, pugi::xml_node _config) : Enemy(_type, _config)
{
	CurrentMovement(IDLE_LEFT_FRONT);
	current = current_animation->GetCurrentFrame();
}
Pinkking::~Pinkking()
{
}




// Actions (SearchWalk, Walk, Attack, Hability 1, Hability 2, Die)
void Pinkking::SearchWalk()
{
	
	nearposition = App->entity_manager->NearestCharacter(position);
	nearposition = App->map->WorldToMap(nearposition.first, nearposition.second);
	App->pathfinding->CreatePathBishop(App->map->WorldToMap(position.first, position.second), nearposition, current_stats.PMove);
	if (timer_skill_1 == 3) {
		current_turn = SEARCH_ABILITY_1;
		timer_skill_1 = 0;
	}
	else
		current_turn = MOVE;
	timer_skill_1++;

}

void Pinkking::Walk(const std::vector<std::pair<int, int>> *_path)
{

	if (App->debug)
	{
		for (uint i = 0; i < _path->size(); ++i)
		{
			std::pair<int, int> pos_debug = App->map->MapToWorld(_path->at(i).first, _path->at(i).second);
			App->render->Blit(debug_texture, pos_debug.first, pos_debug.second, &debug_green);
		}
	}
	if (!App->pathfinding->IsUsed(_path->at(1), this)) {
		if (_path->size() > 1)
		{
			objective_position.push_back(App->map->MapToWorld(_path->at(1).first, _path->at(1).second));
		}

		if (sqrt((nearposition.first - _path->at(0).first)*(nearposition.first - _path->at(0).first) + (nearposition.second - _path->at(0).second)*(nearposition.second - _path->at(0).second)) > current_stats.RangeAtk) {
			if (_path->at(0).first >= _path->at(1).first && _path->at(0).second <= _path->at(1).second) {
				CurrentMovement(WALK_LEFT);
			}
			else if (_path->at(0).first >= _path->at(1).first && _path->at(0).second >= _path->at(1).second) {
				CurrentMovement(WALK_BACK);
			}
			else if (_path->at(0).first <= _path->at(1).first && _path->at(0).second <= _path->at(1).second) {
				CurrentMovement(WALK_FRONT);
			}
			else if (_path->at(0).first <= _path->at(1).first && _path->at(0).second >= _path->at(1).second) {
				CurrentMovement(WALK_RIGHT);
			}
			current_turn = MOVE;
		}
		else {
			current_turn = SEARCH_ATTACK;
			//current_turn = END_TURN;
		}


		if ((objective_position.back().first == position.first && objective_position.back().second == position.second) || (objective_position.back().first == position.first && objective_position.back().second == position.second)) {
			CurrentMovement(IDLE_LEFT_FRONT);

			if (sqrt((nearposition.first - _path->at(0).first)*(nearposition.first - _path->at(0).first) + (nearposition.second - _path->at(0).second)*(nearposition.second - _path->at(0).second)) <= current_stats.RangeAtk)
				current_turn = SEARCH_ATTACK;
			else
				current_turn = END_TURN;


		}
	}
	else {
		if (sqrt((nearposition.first - _path->at(0).first)*(nearposition.first - _path->at(0).first) + (nearposition.second - _path->at(0).second)*(nearposition.second - _path->at(0).second)) <= current_stats.RangeAtk)
			current_turn = SEARCH_ATTACK;
		else
			current_turn = END_TURN;
	}
		
	
	//LOG("current position: x. %i y. %i  objective position: x. %i y. %i", position.first, position.second, objective_position.back().first, objective_position.back().second);
	
		
}

void Pinkking::SearchAttack()
{
	nearposition = App->entity_manager->NearestCharacter(position);
	//App->pathfinding->CreatePath(App->map->WorldToMap(position.first, position.second), App->map->WorldToMap(nearposition.first, nearposition.second));
	
	current_turn = ATTACK;
	//IA Attack. Into range of position + attack. If enemy is near to dead. If enemy def.
}

void Pinkking::Attack(const std::vector<std::pair<int, int>> *_path)
{
	std::pair<int, int> pos = App->map->WorldToMap(position.first, position.second);
	range = App->entity_manager->RangeOfAttack(pos, current_stats.RangeAtk, tiles_range_attk);
	std::pair<int, int> car = App->entity_manager->CharactersPrioritzationAttack(range, tiles_range_attk);
	objective_position.push_back(car);
	current_movement = ATTACK_LEFT_FRONT;

	/*current_movement = ATTACK_LEFT_FRONT;
	if (current_animation->Finished()) {
		App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
		current_movment = IDLE_LEFT_FRONT;
		current_turn = END_TURN;
	}*/

	App->entity_manager->ThrowAttack(objective_position, current_stats.AtkF, ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
	current_turn = END_TURN;
	
}

void Pinkking::SearchAbility_1()
{
	nearposition = App->entity_manager->NearestCharacter(position);
	current_turn = ABILITY_1;
}

void Pinkking::Ability_1()
{
	objective_position.push_back(nearposition);
	std::pair<int, int> tmp_1 = App->map->WorldToMap(position.first, position.second);
	std::pair<int,int> tmp_2 = App->map->WorldToMap(nearposition.first, nearposition.second);
	int dmg = current_stats.AtkS / sqrt((tmp_2.first - tmp_1.first)*(tmp_2.first - tmp_1.first) + (tmp_2.second - tmp_1.second)*(tmp_2.second - tmp_1.second));


	/*current_movement = ABILITY_1_FRONT;
	if (current_animation->Finished()) {
		App->entity_manager->ThrowAttack(objective_position, dmg, ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
		current_movment = IDLE_LEFT_FRONT;
		current_turn = END_TURN;
	}*/

	App->entity_manager->ThrowAttack(objective_position, dmg, ENTITY_TYPE::ENTITY_ENEMY_PINKKING);
	current_turn = END_TURN;
	// blit de la sombra en esa posicion
}


void Pinkking::CurrentMovement(MOVEMENT _movement) {

	switch (_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		current_movement = IDLE_LEFT_FRONT;
		current_animation = &idle_left_front;
		break;
	case Entity::IDLE_RIGHT_FRONT:
		current_movement = IDLE_RIGHT_FRONT;
		current_animation = &idle_right_front;
		flipX = true;
		break;
	case Entity::IDLE_LEFT_BACK:
		current_movement = IDLE_LEFT_BACK;
		current_animation = &idle_left_back;
		break;
	case Entity::IDLE_RIGHT_BACK:
		current_movement = IDLE_RIGHT_BACK;
		current_animation = &idle_right_back;
		break;
	case Entity::WALK_LEFT_FRONT:
		current_movement = WALK_LEFT_FRONT;
		current_animation = &walk_left_front;
		position.first -= 2;
		position.second++;
		break;
	case Entity::WALK_RIGHT_FRONT:
		current_movement = WALK_RIGHT_FRONT;
		current_animation = &walk_right_front;
		position.first += 2;
		position.second++;
		flipX = true;
		break;
	case Entity::WALK_LEFT_BACK:
		current_movement = WALK_LEFT_BACK;
		current_animation = &walk_left_back;
		position.first -= 2;
		position.second--;
		break;
	case Entity::WALK_RIGHT_BACK:
		current_movement = WALK_RIGHT_BACK;
		current_animation = &walk_right_back;
		position.first += 2;
		position.second--;
		break;
	case Entity::WALK_LEFT:
		current_movement = WALK_LEFT;
		current_animation = &walk_left;
		position.first--;
		break;
	case Entity::WALK_RIGHT:
		current_movement = WALK_RIGHT;
		current_animation = &walk_right;
		position.first++ ;
		flipX = true;
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
		break;
	case Entity::ATTACK_RIGHT_FRONT:
		current_movement = ATTACK_RIGHT_FRONT;
		current_animation = &attack_right_front;
		break;
	case Entity::ATTACK_LEFT_BACK:
		current_movement = ATTACK_LEFT_BACK;
		current_animation = &attack_left_back;
		break;
	case Entity::ATTACK_RIGHT_BACK:
		current_movement = ATTACK_RIGHT_BACK;
		current_animation = &attack_right_back;
		break;
	case Entity::ABILITY_1_LEFT_FRONT:
		current_movement = ABILITY_1_LEFT_FRONT;
		current_animation = &ability_1_left_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_RIGHT_FRONT:
		current_movement = ABILITY_1_RIGHT_FRONT;
		current_animation = &ability_1_right_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_LEFT_BACK:
		current_movement = ABILITY_1_LEFT_BACK;
		current_animation = &ability_1_left_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_1_FRONT:
		current_movement = ABILITY_1_FRONT;
		current_animation = &ability_1_front;
		break;
	case Entity::ABILITY_1_RIGHT_BACK: // ME HE QUEDADO AQU�
		current_movement = ABILITY_1_RIGHT_BACK;
		current_animation = &ability_1_right_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_FRONT:
		current_movement = ABILITY_2_LEFT_FRONT;
		current_animation = &ability_2_left_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_FRONT:
		current_movement = ABILITY_2_RIGHT_FRONT;
		current_animation = &ability_2_right_front;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_LEFT_BACK:
		current_movement = ABILITY_2_LEFT_BACK;
		current_animation = &ability_2_left_back;
		current_turn = END_TURN;
		break;
	case Entity::ABILITY_2_RIGHT_BACK:
		current_movement = ABILITY_2_RIGHT_BACK;
		current_animation = &ability_2_right_back;
		current_turn = END_TURN;
		break;
	case Entity::DEAD_LEFT_FRONT:
		current_movement = DEAD_LEFT_FRONT;
		current_animation = &dead_left_front;
		current_state = DEATH;
		break;
	case Entity::DEAD_RIGHT_FRONT:
		current_movement = DEAD_RIGHT_FRONT;
		current_animation = &dead_right_front;
		current_state = DEATH;
		break;
	case Entity::DEAD_LEFT_BACK:
		current_movement = DEAD_LEFT_BACK;
		current_animation = &dead_left_back;
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