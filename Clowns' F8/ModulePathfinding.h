#ifndef __Pathfinding_H__
#define __Pathfinding_H__

#include "Module.h"
#include "Entity.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255


class ModulePathfinding : public Module
{
public:

	ModulePathfinding();

	// Destructor
	~ModulePathfinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const std::pair<int, int>& origin, const std::pair<int, int>& destination);
	//Para los alfiles <3
	int CreatePathBishop(const std::pair<int, int>& origin, const std::pair<int, int>& destination, int Movment);
	//Para las torres <3
	int CreatePathTower(const std::pair<int, int>& origin, const std::pair<int, int>& destination, int Movment);

	int CreatePathKnight(const std::pair<int, int>& origin, const std::pair<int, int>& destination);

	int CreatePathQueen(const std::pair<int, int>& origin, const std::pair<int, int>& destination, int Movment);
	// To request all tiles involved in the last generated path
	const std::vector<std::pair<int, int>>* GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const std::pair<int, int>& pos) const;


	// Utility: returns true is the tile is walkable
	bool IsWalkable(const std::pair<int, int>& pos) const;

	// Utility: returns true is the tile is walkable
	bool CanTrap(const std::pair<int, int>& pos) const;

	// Utility: returns true is the tile is used by otrhe entity
	bool IsUsed(const std::pair<int, int>& pos, Entity* _entity) const;

	// Utility: returns true is the tile is attackable
	bool IsAttackable(const std::pair<int, int>& _pos, ENTITY_TYPE _type) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const std::pair<int, int>& pos) const;



private:

	// size of the map
	uint width = 0u;
	uint height = 0u;
	// all map walkability values [0..255]
	uchar* map;
	// we store the created path here
	std::vector<std::pair<int, int>> last_path;
};

// forward declaration
struct PathList;

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------
struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const std::pair<int, int>& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill) const;
	uint FindWalkableAdjacentsBishop(PathList& list_to_fill, int Movment_Points)const;
	uint FindWalkableAdjacentsTower(PathList& list_to_fill, int Movment_Points) const;
	uint FindWalkableAdjacentsKnight(PathList& list_to_fill) const;
	uint FindWalkableAdjacentsQueen(PathList& list_to_fill, int Movment_Points) const;

	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const std::pair<int, int>& destination);

	// -----------
	int g;
	int h;
	std::pair<int, int> pos;
	const PathNode* parent; // needed to reconstruct the path in the end
};

// ---------------------------------------------------------------------
// Helper struct to include a list of path nodes
// ---------------------------------------------------------------------
struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	const PathNode* Find(const std::pair<int, int>& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	const PathNode* GetNodeLowestScore() const;

	// -----------
	// The list itself, note they are not pointers!
	std::list<PathNode> list;
};



#endif // !__Pathfinding_H__
