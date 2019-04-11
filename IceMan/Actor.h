#ifndef ACTOR_H_
#define ACTOR_H_
#include <memory>
#include <string>
#include "GraphObject.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


//ACTOR CLASS
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* World, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth);
	virtual void doSomething() = 0; //making Actor an abstract class by making doSomething a pure virtual function.
	virtual std::string getItemType() = 0;
	virtual void annoy(int d);
	virtual void bribe();
	~Actor();
	bool playerAtDist(int dist, int i, int j);
	bool protesterAtDist(int dist, std::shared_ptr<Actor>& prot);
	bool isActive();


protected:
	StudentWorld* m_stu_world;
	bool m_active;

private:




};
// ICE CLASS 
class Ice :public Actor
{
public:

	Ice(StudentWorld* World, int Ice_startX, int Ice_startY);
	virtual void doSomething();
	virtual std::string getItemType();
	~Ice();

private:

};

//ICEMAN CLASS
class Iceman : public Actor 
{

public:
	Iceman(StudentWorld* World, int Iceman_startX, int Iceman_startY);
	void virtual doSomething();
	virtual std::string getItemType();
	virtual void annoy(int d);
	int getHP();
	int getWater();
	int getSonar();
	int getGold();
	void addWater();
	void addSonar();
	void addGold();



private:
	int m_hit_points;
	int m_num_water;
	int m_sonar_charges;
	int m_gold_nugget;
	
	void squirtWater();
	void dropNugget();
	void useSonar();

};

//BOULDER CLASS
class Boulder : public Actor
{
public: 
	Boulder(StudentWorld* World, int Boulder_startX, int Boulder_startY);
	virtual void doSomething();
	virtual std::string getItemType();

private:
	//states---I'm wondering if there's a better way to do this?
	bool stable, waiting, falling;
	int waitTimer;
	

};

//SQUIRT CLASS
class Squirt : public Actor
{
public:
	Squirt(StudentWorld* World, int Boulder_startX, int Boulder_startY, Direction d);
	virtual void doSomething();
	virtual std::string getItemType();


private:
	int distTraveled;

};

//BARREL CLASS
class Barrel : public Actor
{
public:
	Barrel(StudentWorld* World, int Barrel_startX, int Barrel_startY);
	virtual void doSomething();
	virtual std::string getItemType();


private:


};
//GOLD NUGGET CLASS
class Nugget : public Actor
{
public:
	Nugget(StudentWorld* World, int Nugget_startX, int Nugget_startY, bool Nugget_Permanent);
	virtual void doSomething();
	virtual std::string getItemType();

private:
	bool m_permanent;
	int m_maxTicks, m_elapsedTicks;
};
//SONAR KIT CLASS
class Sonar : public Actor
{
public:
	Sonar(StudentWorld* World, int Sonar_startX, int Sonar_startY);
	virtual void doSomething();
	virtual std::string getItemType();

private:
	int m_maxTicks, m_elapsedTicks;
};
// WATER POOL CLASS
class Pool : public Actor
{
public:
	Pool(StudentWorld* World, int Pool_startX, int Pool_startY);
	virtual void doSomething();
	virtual std::string getItemType();

private:
	int m_maxTicks, m_elapsedTicks;

};

//PROTESTER CLASS
class Protester : public Actor
{
public:
	Protester(StudentWorld* World, int Prot_startX, int Prot_startY, int imageID); //We should derive the hardcore protester from this class as well.
	virtual void doSomething();
	virtual std::string getItemType();
	virtual bool facingPlayer();
	virtual bool inLineOfSightToPlayer();
	virtual bool pathBlocked(int xCoord, int yCoord);
	virtual bool directionBlocked(Direction dir);
	virtual bool atJunction();
	virtual void turnToFacePlayer();
	virtual void moveInDirection();
	virtual void annoy(int d);
	virtual void bribe();
	virtual Direction getRandomDirection();
	virtual Direction getUpOrDown();
	virtual Direction getLeftOrRight();
	virtual int getSquaresToMove();

protected:
	int m_hit_points, m_numSquaresToMoveInCurrentDirection, m_restingTicks, m_turnTimer, m_ticksRested, m_shoutTimer;
	bool m_leaving, m_resting, m_recentlyShouted, m_recentlyTurned;



private:
//	vector<point> m_PathPoints;


};
//HARDCORE PROTESTER CLASS
class HardcoreProtester : public Protester
{
public: 	
	HardcoreProtester(StudentWorld* World, int HCP_startX, int HCP_startY);
	virtual void doSomething();
	virtual std::string getItemType();

private:
	virtual void bribe();
};
#endif // ACTOR_H_