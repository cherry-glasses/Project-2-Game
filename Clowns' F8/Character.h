#ifndef __Character_H__
#define __Character_H__

#include "Animation.h"
#include "Entity.h"

class Character : public Entity
{
public:
	Character(ENTITY_TYPE _type, pugi::xml_node _config) : Entity(_type, _config) {}

	// Destructor
	virtual ~Character() {}

	// Called before the first frame
	virtual bool Start() { return true; }

	// Called each loop iteration
	virtual bool PreUpdate();
	virtual bool Update(float _dt);

	// Character thinks
	virtual void SearchWalk() {}
	virtual void SelectWalk();
	virtual void Walk();
	virtual void SearchAttack() {}
	virtual void SelectAttack();
	virtual void Attack();
	virtual void SearchAbility_1() {}
	virtual void SelectAbility_1();
	virtual void Ability_1();
	virtual void SearchAbility_2() {}
	virtual void SelectAbility_2();
	virtual void Ability_2();
	virtual void SearchAbility_3() {}
	virtual void SelectAbility_3();
	virtual void Ability_3();
	virtual void Defend();
	virtual void ComeBack();
	virtual void CurrentMovement(MOVEMENT _movement) {}
	virtual void EndTurn();
	virtual void Die();

	//Save and Load
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	//Moves and Attack Select
	virtual void InputSelectMove();

protected:

	int Cap = -1;
	std::list<std::pair<int, int>> possible_mov_list;
	std::list<std::pair<int, int>> inrange_mov_list;
	std::list<std::pair<int, int>> nomov_list;
	std::vector<std::pair<int, int>> possible_map;
	std::pair<int, int> comeback_position;
	MOVEMENT comeback_movement;

	bool finish_attack = false;
	bool sapphire_ability = false;
	
	

};

#endif // !__Character_H__

