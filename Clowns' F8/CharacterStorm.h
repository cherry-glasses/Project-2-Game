#ifndef _CharacterStorm_H__
#define _CharacterStorm_H__

#include "Character.h"

class CharacterStorm : public Character
{
public:

	CharacterStorm(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~CharacterStorm();

	//Move and Attack
	void SearchWalk();
	void SearchAttack();
	void SearchAbility_1();
	void SearchAbility_2();
	void SearchAbility_3();

	void CurrentMovement(MOVEMENT _movement);

	int vampire_count = 0;

};

#endif // !__CharacterStorm_H__
