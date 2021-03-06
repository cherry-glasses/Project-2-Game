#ifndef __ModuleEntityManager_H__
#define __ModuleEntityManager_H__

#include "Module.h"
#include "Entity.h"

class CharacterGeorgeB;

class ModuleEntityManager : public Module
{
public:
	ModuleEntityManager();

	// Destructor
	virtual ~ModuleEntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float _dt);
	bool PostUpdate(float _dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Entity Manager
	Entity* CreateEntity(ENTITY_TYPE type);
	bool DeleteEntity(Entity* entity);
	void OrderEntitiesByAgility();
	void OrderEntitiesByPosition();
	std::pair<int,int> NearestCharacter(std::pair<int, int> myposition);

	std::pair<int, int> CharactersPrioritzationAttack(std::pair<int, int>* AttackRange, int AttackRangeint);

	std::pair<int, int> AiHeals(std::pair<int, int>* AttackRange, int AttackRangeint);

	// Myposition = position, Radius = zone of effect, size: returns the size of the array.
	std::pair<int, int>* RangeOfAttack(std::pair<int, int> myposition, int radius, int& size);
	inline bool InRange(std::pair<int, int> myposition, std::pair<int, int> tiledposition, int radius) { return (sqrt(((tiledposition.first - myposition.first)*(tiledposition.first - myposition.first)) + ((tiledposition.second - myposition.second)*(tiledposition.second - myposition.second))) <= (sqrt((radius*radius)+1))); };

	bool UpdateWalk(std::pair<int, int> tile_id);
	//Returns True if there are characters alive;
	bool ThereAreCharAlive();

	float CalculateDistance(std::pair<int, int> origin, std::pair<int, int> objective);

	void ThrowAttack(std::vector<std::pair<int,int>> _positions, int _damage, ENTITY_TYPE _type, bool _special);

	void ThrowParticleDamage(std::pair<int,int> _pos , int _damage);

	void StartingTurn(Entity* _entity);

	void LevelUP(int _exp);


	//returns true if there are characters around the position
	bool Characters_around(std::pair<int, int> position);
	
public:
	std::list<Entity*> characters;
	std::list<Entity*> enemies;
	std::list<Entity*> entities;
	std::list<Entity*> objects;

	bool paused = false;

	int exp_sapphire = 0;
	int exp_iris = 0;
	int exp_georgeb = 0;
	int exp_storm = 0;

private:
	
	pugi::xml_node entity_configs;

	bool starting = true;
	int stun_fx = 0;
	
};
#endif // !__ModuleEntityManager_H__
