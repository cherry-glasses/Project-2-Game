#ifndef _Entity_H__
#define _Entity_H__

#include "Animation.h"

class Emitter;
struct SDL_Texture;
struct SDL_Rect;

enum class ENTITY_TYPE
{
	ENTITY_CHARACTER_SAPPHIRE,
	ENTITY_CHARACTER_IRIS,
	ENTITY_CHARACTER_STORM,
	ENTITY_CHARACTER_GEORGEB,
	ENTITY_ENEMY_BONEYMAN,
	ENTITY_ENEMY_PINKKING,
	ENTITY_ENEMY_BURGDOG,
	ENTITY_ENEMY_HOTDOG,
	ENTITY_ENEMY_POLARBEAR,
	ENTITY_ENEMY_POLARPATH,
	ENTITY_ENEMY_CHERRYBLACKGLASSES,
	ENTITY_OBJECT_TREE1,
	ENTITY_OBJECT_TREE2,
	ENTITY_OBJECT_TREE3,
	ENTITY_OBJECT_STONE,
	ENTITY_OBJECT_STONE2,
	ENTITY_OBJECT_STONE3,
	ENTITY_OBJECT_STALAGMITE,
	ENTITY_OBJECT_VOLCANO,
	ENTITY_OBJECT_ICE,
	ENTITY_OBJECT_MOUNTAIN,
	ENTITY_OBJECT_MOUNTAIN2,
	ENTITY_OBJECT_BLOODSTREAM,
	ENTITY_OBJECT_BEARTRAP,
	NO_TYPE
};

typedef struct {
	int Hp;
	int Mana;
	int PMove;

	int AtkF;
	int AtkS;
	int RangeAtk;
	int RangeAbility_1;
	int RangeAbility_2;
	int RangeAbility_3;
	int DefF;
	int DefS;
	int Crit;
	int Eva;
	int Agi;

	int Attack;
	int Ability_1;
	int Ability_2;
	int Ability_3;
} Stats;

typedef struct {
	int Hp;
	int Mana;

	int AtkF;
	int AtkS;
	int DefF;
	int DefS;
	int Crit;

} Evolutions;

typedef struct {
	std::string Attack_name;
	std::string Ability_1_name;
	std::string Ability_2_name;
	std::string Ability_3_name;
	
} Attacks_names;

typedef struct {
	int Attack_SFX;
	int Ability_1_SFX;
	int Ability_2_SFX;
	int Ability_3_SFX;
	int	Defend_SFX;
	int	Critic_SFX;
	int Dead_SFX;

} SFX;

class Entity
{

public:
	
	Entity(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	virtual ~Entity() {}

	virtual bool Start() { return true;}
	// Called each loop iteration
	virtual bool PreUpdate() {return true;}
	virtual bool Update(float _dt) {return true;}
	virtual bool PostUpdate(float _dt);
	
	//Move and Attack
	virtual void SearchWalk() {}
	virtual void SearchAttack() {}
	virtual void SearchAbility_1() {}
	virtual void Defend() {}
	virtual void ComeBack() {}
	virtual void EndTurn() {}
	virtual void Die() {}

	// Load and Save
	virtual bool Load(pugi::xml_node& _node);
	virtual bool Save(pugi::xml_node& _node) const;
	
	// Called before quitting
	virtual bool CleanUp();

	virtual void Draw() {}

	// we can change the Entity* by the id of that entity.
	virtual void Skill(Entity* _objective, int _skill_id) {

	}

	void PlaySFX(const int _channel, const int _repeat = 0) const;
	void LoadAnim(pugi::xml_node _config);
	bool LoadAnimation(pugi::xml_node _node, Animation &_anim);

	virtual std::pair<int, int> GetPosition();
	virtual void SetPosition(const float &_x, const float &_y);
	virtual ENTITY_TYPE GetType();


public:

	SDL_Rect portrait;
	Stats default_stats;
	Stats current_stats;
	Evolutions evolution_stats;
	Attacks_names attacks_names;
	SFX sfx;
	bool damaged = false;
	bool defend = false;
	bool stunned = false;
	bool invulnerable = false;
	bool vampire = false;
	bool critic = false;
	bool flipX = false;
	int exp = 0;
	int exp_aux = 0;
	int level = 1;
	std::vector<int> levels;
	std::pair<int, int> target;

	enum STATE { ALIVE, DEATH };
	enum TURN { SEARCH_MOVE, SELECT_MOVE, MOVE, SELECT_ACTION, SEARCH_ATTACK, SELECT_ATTACK, ATTACK, 
		SEARCH_ABILITY_1, SELECT_ABILITY_1, ABILITY_1, 
		SEARCH_ABILITY_2, SELECT_ABILITY_2, ABILITY_2, 
		SEARCH_ABILITY_3, SELECT_ABILITY_3, ABILITY_3, 
		DEFEND, END_TURN, NONE };

	STATE current_state = ALIVE;
	TURN current_turn = NONE;

	SDL_Rect	current = { 0,0,0,0 };

	std::pair<int, int>  position_margin;
	std::string name;
	std::string type_move;

protected:

	enum MOVEMENT {	IDLE_LEFT_FRONT, IDLE_RIGHT_FRONT, IDLE_LEFT_BACK, IDLE_RIGHT_BACK, IDLE_LEFT, IDLE_RIGHT, IDLE_FRONT, IDLE_BACK,
		WALK_LEFT_FRONT, WALK_RIGHT_FRONT,  WALK_LEFT_BACK, WALK_RIGHT_BACK, WALK_LEFT, WALK_RIGHT, WALK_FRONT, WALK_BACK,
		ATTACK_LEFT_FRONT, ATTACK_RIGHT_FRONT, ATTACK_LEFT_BACK, ATTACK_RIGHT_BACK, ATTACK_LEFT, ATTACK_RIGHT, ATTACK_FRONT, ATTACK_BACK,
		ABILITY_1_LEFT_FRONT, ABILITY_1_RIGHT_FRONT, ABILITY_1_LEFT_BACK, ABILITY_1_RIGHT_BACK, ABILITY_1_LEFT, ABILITY_1_RIGHT, ABILITY_1_FRONT, ABILITY_1_BACK,
		ABILITY_2_LEFT_FRONT, ABILITY_2_RIGHT_FRONT, ABILITY_2_LEFT_BACK, ABILITY_2_RIGHT_BACK, ABILITY_2_LEFT, ABILITY_2_RIGHT, ABILITY_2_FRONT, ABILITY_2_BACK,
		ABILITY_3_LEFT_FRONT, ABILITY_3_RIGHT_FRONT, ABILITY_3_LEFT_BACK, ABILITY_3_RIGHT_BACK, ABILITY_3_LEFT, ABILITY_3_RIGHT, ABILITY_3_FRONT, ABILITY_3_BACK,
		DEFEND_LEFT_FRONT, DEFEND_RIGHT_FRONT, DEFEND_LEFT_BACK, DEFEND_RIGHT_BACK, DEFEND_LEFT, DEFEND_RIGHT, DEFEND_FRONT, DEFEND_BACK,
		DEAD_LEFT_FRONT, DEAD_RIGHT_FRONT, DEAD_LEFT_BACK, DEAD_RIGHT_BACK, DEAD_LEFT, DEAD_RIGHT, DEAD_FRONT, DEAD_BACK, DEAD_DEFAULT,
	};

	MOVEMENT current_movement = IDLE_LEFT_BACK;
	
	Animation * current_animation = nullptr;

	Animation idle_left_back;
	Animation idle_right_back;
	Animation idle_left_front;
	Animation idle_right_front;
	Animation idle_left;
	Animation idle_right;
	Animation idle_front;
	Animation idle_back;
	Animation walk_left_back;
	Animation walk_right_back;
	Animation walk_left_front;
	Animation walk_right_front;
	Animation walk_left;
	Animation walk_right;
	Animation walk_right_2;
	Animation walk_front;
	Animation walk_front_2;
	Animation walk_back;
	Animation walk_back_2;
	Animation attack_left_back;
	Animation attack_right_back;
	Animation attack_left_front;
	Animation attack_right_front;
	Animation attack_left;
	Animation attack_right;
	Animation attack_front;
	Animation attack_back;
	Animation ability_1_left_back;
	Animation ability_1_right_back;
	Animation ability_1_left_front;
	Animation ability_1_right_front;
	Animation ability_1_left;
	Animation ability_1_right;
	Animation ability_1_front;
	Animation ability_1_back;
	Animation ability_2_left_back;
	Animation ability_2_right_back;
	Animation ability_2_left_front;
	Animation ability_2_right_front;
	Animation ability_2_left;
	Animation ability_2_right;
	Animation ability_2_front;
	Animation ability_2_back;
	Animation ability_3_left_back;
	Animation ability_3_right_back;
	Animation ability_3_left_front;
	Animation ability_3_right_front;
	Animation ability_3_left;
	Animation ability_3_right;
	Animation ability_3_front;
	Animation ability_3_back;
	Animation defend_left_back;
	Animation defend_right_back;
	Animation defend_left_front;
	Animation defend_right_front;
	Animation defend_left;
	Animation defend_right;
	Animation defend_front;
	Animation defend_back;
	Animation dead_left_back;
	Animation dead_right_back;
	Animation dead_left_front;
	Animation dead_right_front;
	Animation dead_left;
	Animation dead_right;
	Animation dead_front;
	Animation dead_back;

	ENTITY_TYPE type = ENTITY_TYPE::NO_TYPE;
	
	SDL_Texture* entity_texture = nullptr;
	SDL_Texture* debug_texture = nullptr;
	SDL_Rect debug_green;
	SDL_Rect debug_red;
	SDL_Rect debug_blue;
	SDL_Rect debug_yellow;
	SDL_Rect circle_green;
	SDL_Rect circle_red;
	SDL_Rect circle_blue;
	SDL_Rect circle_yellow;

	std::pair<int, int>  position;
	std::vector<std::pair<int, int>>  objective_position;
	int tiles_range_attk = 0;
	std::pair<int, int>* range;

	int start = 0;

	bool istargeted = false;

	bool sound_fx = false;

	Emitter* emitter = nullptr;
	
	
};

#endif // !_Entity_H_

