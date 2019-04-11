#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <queue>

#include <stack>
#include <vector>
#include <string>
#include <memory>
const int Max_X = 64;
const int Max_Y = 64;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{

	}
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool containsIce(int xCoord, int yCoord); //returns true if there is an ice with these coordinates//
	bool containsBoulder(int xCoord, int yCoord); //returns true if there is a boulder within these coordinates//
	bool containsPlayer(int xCoord, int yCoord);  //returns true if there is an Iceman within these coordinates//
	bool containsProtester(int xCoord, int yCoord, std::shared_ptr<Actor>& prot);  //returns true if there is a Protester within these coordinates//
	void removeIce(int xCoord, int yCoord); //removes ice at this point
	void createSquirt(int xCoord, int yCoord, const GraphObject::Direction dir);
	void createNugget(int xCoord, int yCoord, bool isPermanent);
	void createBoulder(int xCoord, int yCoord);
	void createOil(int xCoord, int yCoord);
	void createSonar(int xCoord, int yCoord);
	void createPool(int xCoord, int yCoord);
	void createProtester(int xCoord, int yCoord, bool hardcore);
	void possibleProtester(int chance);
	std::string updateStatUI();
	void collectWater();
	void collectSonar();
	void collectGold();
	void collectOil();
	void sonarReveal();
	int getNumOil();
	int setBoulderX();
	int setBoulderY();
	int setPickupX();
	int setPickupY();
	int setPoolX();
	int setPoolY();
	int getPlayerX();
	int getPlayerY();
	void possibleGoodie();
	void annoyPlayer(int d);
	void annoyProtesters(std::shared_ptr<Actor> prot, int d);
	void bribeProtesters(std::shared_ptr<Actor> prot);
	bool objectsAtDist(std::shared_ptr<Actor>& a, int i, int j, int d);
	bool isWalkablePoint(int xCoord, int yCoord);
	void findWalkablePoints(int arr[60][60]);
//	point* breadthFirstSearch(point* origin, point* target);


private:
	 


	int m_num_oil, m_num_boulders, m_num_gold, m_num_protesters, m_protester_timer, m_protester_elapsed_time, m_chance_of_hardcore, m_current_protesters;
	
	//Ice is going to be stored in a 2d-array/
	std::unique_ptr<Ice> m_iceField[Max_X][Max_Y];
	int m_walkablePoints[60][60];

	//Iceman shouldn't be in a container//
	std::unique_ptr<Iceman> m_player;

	//All other items are going to be in a single vector//
	std::vector<std::shared_ptr<Actor>> m_ItemsAndProtesters;

};
/*
struct point;

class walkableTree
{
public:
	walkableTree(StudentWorld* w) 
	{ 
		origin = nullptr;
		m_world = w;
	}
	~walkableTree();



private:
	point* origin;
	StudentWorld* m_world;
	void insert(point*& p, int x, int y, int d);
	void destroy(point*& p);
};
*/

#endif // STUDENTWORLD_H_