#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* World, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth) {
	m_stu_world = World;
}

// I stopped here
Actor:: ~Actor()
{

}
bool Actor::playerAtDist(int dist, int i, int j)
{
	//check a square to see if the player is nearby//
	for (int x = -dist; x <= dist; x++)
	{
		for (int y = -dist; y <= dist; y++)
		{
			if (m_stu_world->containsPlayer(i + x, j + y))
			{
				return true;
			}
		}
	}
	return false; //if we haven't returned true after checking the square
}
bool Actor::protesterAtDist(int dist, std::shared_ptr<Actor>& prot)
{
	std::shared_ptr<Actor> p = NULL;
	//check a square of distxdist to see if the player is nearby//
	for (int x = 0; x <= dist; x++)
	{
		for (int y = 0; y <= dist; y++)
		{
			if (m_stu_world->containsProtester(getX() + x, getY() + y, p))
			{
				prot = p;
				return true;
			}
		}
	}
	return false; //if we haven't returned true after checking the square
}
bool Actor::isActive()
{
	return m_active;
}
void Actor::annoy(int d)
{
	return;
}
void Actor::bribe()
{
	return;
}
Ice::Ice(StudentWorld* World, int Ice_startX, int Ice_startY) :Actor(World, Ice_startX, Ice_startY, IID_ICE, right, 0.25, 3)
{
	setVisible(true); 
}
Ice::~Ice() 
{

}
void Ice::doSomething() {
	//This function does not do anything 
}

string Ice::getItemType()
{
	return "Ice";
}

Iceman::Iceman(StudentWorld* World, int Iceman_startX, int Iceman_startY) :Actor(World, Iceman_startX, Iceman_startY, IID_PLAYER, right, 1.0, 0)
{
	m_hit_points = 10;
	m_num_water = 5;
	m_sonar_charges = 1;
	m_gold_nugget = 0;
	setVisible(true);
	m_active = true;
}

void Iceman::doSomething() {
	int key;
	if (m_hit_points == 0)
		return;


	if (m_stu_world->getKey(key))
	{
		if (key == KEY_PRESS_LEFT && getX() > 0)
		{
			for (int y = 0; y < 4; y++)
			{
				if (m_stu_world->containsBoulder(getX() - 1, getY() + y))
					return;
			}
			if (getDirection() == left)
			{
				moveTo(getX() - 1, getY());
				
			}
			else
				setDirection(left);
		}
		if (key == KEY_PRESS_RIGHT && getX() < 60)
		{
			for (int y = 0; y < 4; y++)
			{
				if (m_stu_world->containsBoulder(getX() + 4, getY() + y))
					return;
			}
			
			if (getDirection() == right)
			{
				moveTo(getX() + 1, getY());
				
			}
			else
				setDirection(right);
		}
		if (key == KEY_PRESS_DOWN && getY() > 0)
		{
			for (int x = 0; x < 4; x++)
			{
				if (m_stu_world->containsBoulder(getX() + x, getY()  - 1))
					return;
			}
			if (getDirection() == down)
			{
				moveTo(getX(), getY() - 1);
				
			}
			else
				setDirection(down);
		}
		if (key == KEY_PRESS_UP && getY() < 60)  //Iceman starts at row 60, should never go above that
		{
			for (int x = 0; x < 4; x++)
			{
				if (m_stu_world->containsBoulder(getX()  + x, getY() + 4))
					return;
			}
			if (getDirection() == up)
			{
				moveTo(getX(), getY() + 1);
				
			}
			else
				setDirection(up);
		}
		if (key == KEY_PRESS_SPACE && m_num_water > 0)
		{
			squirtWater();
		}
		if (key == KEY_PRESS_TAB && m_gold_nugget > 0)
		{
			dropNugget();
		}
		if (key == 'z' && m_sonar_charges > 0)
		{
			useSonar();
		}
		if (key == KEY_PRESS_ESCAPE)
		{
			m_hit_points = 0;
		}


	}
	//if iceman overlaps with ice, delete ice and play digging sound//

	//iceman is 4x4 tiles, so we need to check all of them
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			if (m_stu_world->containsIce(this->getX()+i, this->getY()+j))
			{
				m_stu_world->removeIce(this->getX()+i, this->getY()+j);
				GameController::getInstance().playSound(SOUND_DIG);
			}
	}

}
string Iceman::getItemType()
{
	return "Iceman";
}
void Iceman::squirtWater()
{
	if (getDirection() == up)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_stu_world->containsIce(getX() + x, getY() + 4) == true)
				break;

			else
			{
				m_stu_world->createSquirt(getX(), getY() + 4, this->getDirection());
				break;
			}

		}

	}
	if (getDirection() == down)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_stu_world->containsIce(getX() + x, getY() - 1) == true)
				break;
				
			else
			{
				m_stu_world->createSquirt(getX(), getY() - 1, this->getDirection());
				break;
			}

		}
	}
	if (getDirection() == left)
	{
		for (int y = 0; y < 4; y++)
		{
			if (m_stu_world->containsIce(getX() - 1, getY() + y) == true)
				break;

			else
			{
				m_stu_world->createSquirt(getX() - 1, getY(), this->getDirection());
				break;
			}

		}
	}
	if (getDirection() == right)
	{
		for (int y = 0; y < 4; y++)
		{
			if (m_stu_world->containsIce(getX() + 4, getY() + y) == true)
				break;

			else
			{
				m_stu_world->createSquirt(getX() + 4, getY(), this->getDirection());
				break;
			}

		}
	}	
	m_stu_world->playSound(SOUND_PLAYER_SQUIRT);
	m_num_water--;
}
void Iceman::dropNugget()
{
	m_stu_world->createNugget(getX(), getY(), false);
	m_gold_nugget--;
}
void Iceman::useSonar()
{
	m_sonar_charges--;
	m_stu_world->sonarReveal();
	m_stu_world->playSound(SOUND_SONAR);
}
int Iceman::getHP()
{
	return m_hit_points;
}	
int Iceman::getWater()
{
	return m_num_water;
}
int Iceman::getSonar()
{
	return m_sonar_charges;
}
int Iceman::getGold()
{
	return m_gold_nugget;
}
void Iceman::addWater()
{
	m_num_water += 5;
}
void Iceman::addSonar()
{
	m_sonar_charges++;
}
void Iceman::addGold()
{
	m_gold_nugget++;
}
void Iceman::annoy(int d)
{
	m_hit_points -= d;
}
Boulder::Boulder(StudentWorld* World, int Boulder_startX, int Boulder_startY) : Actor(World, Boulder_startX, Boulder_startY, IID_BOULDER, down, 1.0, 1)
{
	stable = true;
	waiting = false;
	falling = false;
	m_active = true;
	waitTimer = 0;
	setVisible(true);
}


void Boulder::doSomething()
{
	if (!m_active)
		return;


	if (stable == true) //check for ice beneath.  if there is no ice below the rock, switch to waiting.
	{
		//checking ice
		for (int x = 0; x < 4; x++)
		{
			if (m_stu_world->containsIce(this->getX() + x, this->getY() - 1))
				return;
		}
		stable = false;
		waiting = true;
	}

	else if (waiting == true)
	{
		if (waitTimer < 100)
			waitTimer++;
		else
		{
			waiting = false;
			falling = true;
			m_stu_world->playSound(SOUND_FALLING_ROCK);
		}
	}
	else if (falling == true)//fall until it hits ice, a boulder, or the bottom of the game
	{
		//checking for items below, or the bottom of the game
		for (int x = 0; x < 4; x++)
		{
			if (m_stu_world->containsIce(this->getX() + x, this->getY() - 1) || this->getY() == 0 || m_stu_world->containsBoulder(this->getX() + x, this->getY() - 1))
			{
				m_active = false;
			}
		}
		moveTo(getX(), getY() - 1);
		

	
		std::shared_ptr<Actor> p = NULL;

		if (playerAtDist(3, getX(), getY()))
		{
			m_stu_world->annoyPlayer(100);
		}
		if (protesterAtDist(3, p))
		{
			m_stu_world->annoyProtesters(p, 100);
			m_stu_world->increaseScore(500 /*points*/);
		}

	}
}

string Boulder::getItemType()
{
	return "Boulder";
}	

Squirt::Squirt(StudentWorld* World, int Squirt_startX, int Squirt_startY, Direction d) : Actor(World, Squirt_startX, Squirt_startY, IID_WATER_SPURT, d, 1.0, 1)
{
	distTraveled = 0;
	m_active = true;
	setVisible(true);
}
void Squirt::doSomething()
{
	std::shared_ptr<Actor> p = NULL;

	if (m_active == false)
	{
		return;
	}
	else if (protesterAtDist(3, p))
	{
		m_stu_world->annoyProtesters(p, 2);
		m_active = false;
		return;
	}
	else if (distTraveled < 4)
	{
		if (getDirection() == left)
		{
			moveTo(getX() - 1, getY());
		}
		else 
			if (getDirection() == right)
		{
			moveTo(getX() + 1, getY());
		}
		else if (getDirection() == down)
		{
			moveTo(getX(), getY() - 1);
		}
		else if (getDirection() == up)
		{
			moveTo(getX(), getY() + 1);
		}
		distTraveled++;
	}
	else if (distTraveled == 4)
	{
		m_active = false;
	}
}
string Squirt::getItemType()
{
	 return "Squirt";
}
Barrel::Barrel(StudentWorld* World, int Barrel_startX, int Barrel_startY) : Actor(World, Barrel_startX, Barrel_startY, IID_BARREL, right, 1.0, 2)
{
	m_active = true;
	setVisible(false);
}
void Barrel::doSomething()
{
	if (m_active == false)
		return;

	if (!isVisible() && playerAtDist(4, getX(), getY()))
	{
		setVisible(true);
		return;
	}

	if (playerAtDist(3, getX(), getY()))
	{
		m_active = false;
		GameController::getInstance().playSound(SOUND_FOUND_OIL);
		m_stu_world->collectOil();
		m_stu_world->increaseScore(1000 /*points*/);
	}

}
std::string Barrel::getItemType()
{
	return "Barrel";
}

Nugget::Nugget(StudentWorld* World, int Nugget_startX, int Nugget_startY, bool Nugget_Permanent) : Actor(World, Nugget_startX, Nugget_startY, IID_GOLD, right, 1.0, 2)
{
	m_active = true;
	m_permanent = Nugget_Permanent;
	if (m_permanent)
		setVisible(false);//If it is permanent, it should be hidden from the player at the beginning, and vice-versa
	else if (!m_permanent)
		setVisible(true);

	m_maxTicks = 100;
	m_elapsedTicks = 0;
}
void Nugget::doSomething()
{
	if (!m_active)
		return;

	if (m_permanent) //this loop is for when the nugget is permanent, meaning the ice man can pick it up
	{
		if (!isVisible() && playerAtDist(4, getX(), getY()))
		{
			setVisible(true);
			return;
		}

		if (playerAtDist(3, getX(), getY()))
		{
			m_active = false;
			GameController::getInstance().playSound(SOUND_GOT_GOODIE);
			m_stu_world->collectGold();
			m_stu_world->increaseScore(10 /*points*/);
			//ICEMAN NUGGETS INCREASES BY 1//
		}
	}
	else if (!m_permanent)
	{
		m_elapsedTicks++;

		if (m_elapsedTicks >= m_maxTicks)
		{
			m_active = false;
		}

		std::shared_ptr<Actor> p = NULL;
		if (protesterAtDist(3, p))
		{
			m_active = false;
			GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
			m_stu_world->bribeProtesters(p);
			m_stu_world->increaseScore(25 /*points*/);
		}
	}
}
std::string Nugget::getItemType()
{
	return "Nugget";
}
Sonar::Sonar(StudentWorld* World, int Sonar_startX, int Sonar_startY) : Actor(World, Sonar_startX, Sonar_startY, IID_SONAR, right, 1.0, 2)
{
	setVisible(true);
	m_active = true;
	m_maxTicks = 300 - 10*m_stu_world->getLevel();
	if (m_maxTicks < 100)
		m_maxTicks = 100;

	m_elapsedTicks = 0;
}
void Sonar::doSomething()
{
	m_elapsedTicks++;
	if (!m_active)
		return;

	if (playerAtDist(3, getX(), getY()))
	{
		m_active = false;
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		m_stu_world->collectSonar();
		m_stu_world->increaseScore(75 /*points*/);
	}

	if (m_elapsedTicks >= m_maxTicks)
	{
		m_active = false;
	}
}
std::string Sonar::getItemType()
{
	return "Sonar Kit";
}
Pool::Pool(StudentWorld* World, int Pool_startX, int Pool_startY) : Actor(World, Pool_startX, Pool_startY, IID_WATER_POOL, right, 1.0, 2)
{
	setVisible(true);
	m_active = true;
	m_maxTicks = 300 - 10 * m_stu_world->getLevel();
	if (m_maxTicks < 100)
		m_maxTicks = 100;

	m_elapsedTicks = 0;
}
void Pool::doSomething()
{
	m_elapsedTicks++;
	if (!m_active)
		return;

	if (playerAtDist(3, getX(), getY()))
	{
		m_active = false;
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		m_stu_world->collectWater();
		m_stu_world->increaseScore(100 /*points*/);
	}

	if (m_elapsedTicks >= m_maxTicks)
	{
		m_active = false;
	}
}
std::string Pool::getItemType()
{
	return "Water Pool";
}




Protester::Protester(StudentWorld* World, int Prot_startX, int Prot_startY, int imageID) : Actor(World, Prot_startX, Prot_startY, imageID, left, 1.0, 0)
{
	setVisible(true);
	m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
	m_hit_points = 5;
	m_restingTicks = (3 - (m_stu_world->getLevel() / 4));
	if (m_restingTicks < 0)
		m_restingTicks = 0;
	m_shoutTimer = 0;
	m_turnTimer = 0;
	m_ticksRested = 0;
	m_leaving = false;
	m_resting = true;
	m_recentlyShouted = false;
	m_recentlyTurned = false;
}

void Protester::doSomething()
{

	if (!m_active)
		return;

	if (m_resting)
	{
		if (m_ticksRested < m_restingTicks)
		{
			m_ticksRested++;
			return;
		}
		else if (m_ticksRested >= m_restingTicks)
		{
			m_ticksRested = 0;
			m_resting = false;
		}
	}

	if (m_leaving)
	{
		//GET SHORTEST PATH. 
		return;
	}

	else if(!m_resting) //THE PROTESTER IS NOT LEAVING OR RESTING IN THIS BLOCK
	{
		m_resting = true;
		if (m_shoutTimer < 15)
			m_shoutTimer++;
		else if (m_shoutTimer >= 15)
		{
			m_recentlyShouted = false;
		}

		if (m_turnTimer < 200)
		{
			m_turnTimer++;
		}
		else if (m_turnTimer >= 200)
		{
			m_recentlyTurned = false;
		}


		if (playerAtDist(3, getX(), getY()) && facingPlayer() && !m_recentlyShouted)
		{
			m_stu_world->playSound(SOUND_PROTESTER_YELL);
			m_stu_world->annoyPlayer(2);
			m_recentlyShouted = true;
			m_shoutTimer = 0;
			return;
		}

		else if (inLineOfSightToPlayer() && !pathBlocked(m_stu_world->getPlayerX(), m_stu_world->getPlayerY()))
		{
				turnToFacePlayer();
				moveInDirection();
				m_numSquaresToMoveInCurrentDirection = 0;
				return;
		}
		else if(!inLineOfSightToPlayer() && pathBlocked(m_stu_world->getPlayerX(), m_stu_world->getPlayerY()))
		{
			m_numSquaresToMoveInCurrentDirection--;
			if (m_numSquaresToMoveInCurrentDirection <= 0)
			{
				Direction newDirection = getRandomDirection();
				while (directionBlocked(newDirection))
				{
					newDirection = getRandomDirection();
				}
				setDirection(newDirection);
				m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
			}
		}
		
		if (atJunction() && !m_recentlyTurned)
		{
			if (getDirection() == left || getDirection() == right)
			{
				if (directionBlocked(up) && !directionBlocked(down))
				{
					setDirection(down);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
					
				}
				else if (directionBlocked(down) && !directionBlocked(up))
				{
					setDirection(up);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				else
				{
					setDirection(getUpOrDown());
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				m_recentlyTurned = true;
			}
			else if (getDirection() == up || getDirection() == down)
			{
				if (directionBlocked(left) && !directionBlocked(right))
				{
					setDirection(right);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				else if (directionBlocked(right) && !directionBlocked(left))
				{
					setDirection(left);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				else
				{
					setDirection(getLeftOrRight());
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				m_recentlyTurned = true;
			}
		}
		
		moveInDirection();
		m_numSquaresToMoveInCurrentDirection--;
		if (directionBlocked(getDirection()))
		{
			m_numSquaresToMoveInCurrentDirection = 0;
		}
	}



}
std::string Protester::getItemType()
{
	return "Protester";
}
bool Protester::facingPlayer()
{
	bool withinSight = false;
	
	if (getDirection() == left)
	{
		for (int x = 0; x < getX(); x++)
		{
			for (int y = -3; y < 4; y++)
			{
				if (m_stu_world->containsPlayer(x, getY() + y))
				{
					withinSight = true;
					break;
				}

			}
			if (withinSight)
				break;
		}

		if (getX() >= m_stu_world->getPlayerX() && withinSight)
			return true;
		else
			return false;
	}

	if (getDirection() == right)
	{
		for (int x = getX(); x < Max_X; x++)
		{
			for (int y = -3; y < 4; y++)
			{
				if (m_stu_world->containsPlayer(x, getY() + y))
				{
					withinSight = true;
					break;
				}

			}
			if (withinSight)
				break;
		}

		if (getX() <= m_stu_world->getPlayerX() && withinSight)
			return true;
		else
			return false;
	}

	if (getDirection() == up)
	{
		for (int y = getY(); y < Max_Y; y++)
		{
			for (int x = -3; x < 4; x++)
			{
				if (m_stu_world->containsPlayer(getX() + x, getY()))
				{
					withinSight = true;
					break;
				}

			}
			if (withinSight)
				break;
		}

		if (getY() <= m_stu_world->getPlayerY() && withinSight)
			return true;
		else
			return false;
	}

	if (getDirection() == down)
	{
		for (int y = 0; y < getY(); y++)
		{
			for (int x = -3; x < 4; x++)
			{
				if (m_stu_world->containsPlayer(getX() + x, getY()))
				{
					withinSight = true;
					break;
				}

			}
			if (withinSight)
				break;
		}

		if (getY() >= m_stu_world->getPlayerY() && withinSight)
			return true;
		else
			return false;
	}
	return false;
}	
bool Protester::pathBlocked(int xCoord, int yCoord)
{
	//horizontally checking
	if (yCoord == getY()) //this means the target is horizontal from us
	{
		if (xCoord < getX()) //if its to the left
		{
			for (int x = xCoord; x < getX(); x++)
			{
				if (m_stu_world->containsBoulder(x, getY()) || m_stu_world->containsIce(x, getY()))
					return true;
			}
			return false; //if we sucessfully made it out of that loop, there are no obstacles.
		}
		else if (xCoord > getX()) //to the right
		{
			for (int x = getX(); x < xCoord; x++)
			{
				if (m_stu_world->containsBoulder(x, getY()) || m_stu_world->containsIce(x, getY()))
					return true;
			}
			return false; //if we sucessfully made it out of that loop, there are no obstacles.
		}
	}

	//vertically checking
	else if (xCoord == getX()) //this means target is vertical from us
	{
		if (yCoord > getY()) //above
		{
			for (int y = getY(); y < yCoord; y++)
			{
				if (m_stu_world->containsBoulder(getX(), y) || m_stu_world->containsIce(getX(), y))
					return true;
			}
			return false; //if we sucessfully made it out of that loop, there are no obstacles.
		}
		
		else if (yCoord < getY()) //below
		{
			for (int y = yCoord; y < getY(); y++)
			{
				if (m_stu_world->containsBoulder(getX(), y) || m_stu_world->containsIce(getX(), y))
					return true;
			}
			return false; //if we sucessfully made it out of that loop, there are no obstacles.
		}
	}

	//diagonal?  NOPE CANT DO THAT
	//if neither x nor y are in the same line, that means we're diagonal, which means this function is useless.
	return true;
}	
bool Protester::inLineOfSightToPlayer()
{
	//Check horizontal distances.
	for (int x = 0; x < Max_X; x++)
	{
			if (m_stu_world->containsPlayer(x, getY()))
				return true;
	}


	//Check vertical distances.
	for (int y = 0; y < Max_Y; y++)
	{
			if (m_stu_world->containsPlayer(getX(), y))
				return true;
	}

	return false;
}
void Protester::turnToFacePlayer()
{
	if (getY() == m_stu_world->getPlayerY())
	{
		if (getX() < m_stu_world->getPlayerX())
		{
			setDirection(right);
			return;
		}

		else if (getX() > m_stu_world->getPlayerX())
		{
			setDirection(left);
			return;
		}
	}

	else if (getX() == m_stu_world->getPlayerX())
	{
		if (getY() < m_stu_world->getPlayerY())
		{
			setDirection(up);
			return;
		}

		else if (getY() > m_stu_world->getPlayerY())
		{
			setDirection(down);
			return;
		}
	}
	
	else
		return;
}
void Protester::moveInDirection()
{
	if (getDirection() == left)
	{
		if(!directionBlocked(left))
			moveTo(getX() - 1, getY());
	}

	else if (getDirection() == right)
	{
		if (!directionBlocked(right))
			moveTo(getX() + 1, getY());
	}

	else if (getDirection() == up)
	{
		if (!directionBlocked(up))
			moveTo(getX(), getY() + 1);
	}	

	else if (getDirection() == down)
	{
		if (!directionBlocked(down))
			moveTo(getX(), getY() - 1);
	}
}
GraphObject::Direction Protester::getRandomDirection()
{
	int val = rand() % 4 + 1;
	
	if (val == 1)
		return left;
	if (val == 2)
		return right;
	if (val == 3)
		return up;
	if (val == 4)
		return down;
}
bool Protester::directionBlocked(Direction dir)
{
	if (dir == left)
	{
		for (int y = 0; y < 4; y++)
		{
			if (m_stu_world->containsBoulder(getX() - 1, getY() + y) || m_stu_world->containsIce(getX() - 1, getY() + y) || getX() <= 0)
				return true;
		}
		return false;
	}
	else if (dir == right)
	{
		for (int y = 0; y < 4; y++)
		{
			if (m_stu_world->containsBoulder(getX() + 4, getY() + y) || m_stu_world->containsIce(getX() + 4, getY() + y) || getX() >= 60)
				return true;
		}
		return false;
	}
	else if (dir == up)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_stu_world->containsBoulder(getX() + x, getY() + 4) || m_stu_world->containsIce(getX() + x, getY() + 4) || getY() >= 60)
				return true;
		}
			return false;
	}
	else if (dir == down)
	{
		for (int x = 0; x < 4; x++)
		{
			if (m_stu_world->containsBoulder(getX(), getY() - 1) || m_stu_world->containsIce(getX(), getY() - 1) || getY() <= 0)
				return true;
		}
			return false;
	}
}
int Protester::getSquaresToMove()
{
	int squares = rand() % 52 + 8; //[8, 60]
	return squares;
}
bool Protester::atJunction()
{
	Direction dir = getDirection();
	if (dir == left || dir == right)
	{
		for (int x = 0; x < 3; x++)
		{
			if (m_stu_world->containsBoulder(getX() + x, getY() + 4) || m_stu_world->containsIce(getX() + x, getY() + 4))
				return false;
			if (m_stu_world->containsBoulder(getX() + x, getY() - 1) || m_stu_world->containsIce(getX() + x, getY() - 1))
				return false;
		}
		return true;
	}
	else if (dir == up || dir == down)
	{
		for (int y = 0; y < 3; y++)
		{
			if (m_stu_world->containsBoulder(getX() - 1, getY()  + y) || m_stu_world->containsIce(getX() - 1, getY() + y))
				return false;
			if (m_stu_world->containsBoulder(getX() + 4, getY() + y) || m_stu_world->containsIce(getX() + 4, getY() + y))
				return false;
		}
		return true;
	}
}
GraphObject::Direction Protester::getUpOrDown()
{
	int dir = rand() % 2; //[0, 1]
	if (dir == 0)
		return up;
	if (dir == 1)
		return down;
}
GraphObject::Direction Protester::getLeftOrRight()
{
	int dir = rand() % 2; //[0, 1]
	if (dir == 0)
		return left;
	if (dir == 1)
		return right;
}
void Protester::annoy(int d)
{
	m_hit_points -= d;

	if (m_hit_points > 0)
	{
		m_stu_world->playSound(SOUND_PROTESTER_ANNOYED);
		m_ticksRested = -(100 - m_stu_world->getLevel() * 10);
		if (m_ticksRested <= -50)
			m_ticksRested = -50;
	}

	else if (m_hit_points <= 0)
	{
		m_leaving = true;
		m_stu_world->playSound(SOUND_PROTESTER_GIVE_UP);
		m_restingTicks = 0;
	}
}
void Protester::bribe()
{
	m_stu_world->playSound(SOUND_PROTESTER_FOUND_GOLD);
	m_stu_world->increaseScore(25 /*points*/);
	m_leaving = true;
}
HardcoreProtester::HardcoreProtester(StudentWorld* World, int HCP_startX, int HCP_startY) : Protester(World, HCP_startX, HCP_startY, IID_HARD_CORE_PROTESTER)
{
	m_hit_points = 20;
	m_restingTicks = (3 - (m_stu_world->getLevel() / 4));
	if (m_restingTicks < 0)
		m_restingTicks = 0;
}
void HardcoreProtester::doSomething()
{
	if (!m_active)
		return;

	if (m_resting)
	{
		if (m_ticksRested < m_restingTicks)
		{
			m_ticksRested++;
			return;
		}
		else if (m_ticksRested >= m_restingTicks)
		{
			m_ticksRested = 0;
			m_resting = false;
		}
	}

	if (m_leaving)
	{
//		point* currentPoint = new point(getX(), getY(), 0);

//		m_stu_world->breadthFirstSearch(currentPoint,  )
		return;
	}

	else if (!m_resting) //THE PROTESTER IS NOT LEAVING OR RESTING IN THIS BLOCK
	{
		m_resting = true;
		if (m_shoutTimer < 15)
			m_shoutTimer++;
		else if (m_shoutTimer >= 15)
		{
			m_recentlyShouted = false;
		}

		if (m_turnTimer < 200)
		{
			m_turnTimer++;
		}
		else if (m_turnTimer >= 200)
		{
			m_recentlyTurned = false;
		}


		if (playerAtDist(3, getX(), getY()) && facingPlayer() && !m_recentlyShouted)
		{
			m_stu_world->playSound(SOUND_PROTESTER_YELL);
			m_stu_world->annoyPlayer(2);
			m_recentlyShouted = true;
			m_shoutTimer = 0;
			return;
		}

		else if (inLineOfSightToPlayer() && !pathBlocked(m_stu_world->getPlayerX(), m_stu_world->getPlayerY()))
		{
			turnToFacePlayer();
			moveInDirection();
			m_numSquaresToMoveInCurrentDirection = 0;
			return;
		}
		else if (!inLineOfSightToPlayer() && pathBlocked(m_stu_world->getPlayerX(), m_stu_world->getPlayerY()))
		{
			m_numSquaresToMoveInCurrentDirection--;
			if (m_numSquaresToMoveInCurrentDirection <= 0)
			{
				Direction newDirection = getRandomDirection();
				while (directionBlocked(newDirection))
				{
					newDirection = getRandomDirection();
				}
				setDirection(newDirection);
				m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
			}
		}

		if (atJunction() && !m_recentlyTurned)
		{
			if (getDirection() == left || getDirection() == right)
			{
				if (directionBlocked(up) && !directionBlocked(down))
				{
					setDirection(down);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;

				}
				else if (directionBlocked(down) && !directionBlocked(up))
				{
					setDirection(up);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				else
				{
					setDirection(getUpOrDown());
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				m_recentlyTurned = true;
			}
			else if (getDirection() == up || getDirection() == down)
			{
				if (directionBlocked(left) && !directionBlocked(right))
				{
					setDirection(right);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				else if (directionBlocked(right) && !directionBlocked(left))
				{
					setDirection(left);
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				else
				{
					setDirection(getLeftOrRight());
					m_numSquaresToMoveInCurrentDirection = getSquaresToMove();
					m_turnTimer = 0;
				}
				m_recentlyTurned = true;
			}
		}

		moveInDirection();
		m_numSquaresToMoveInCurrentDirection--;
		if (directionBlocked(getDirection()))
		{
			m_numSquaresToMoveInCurrentDirection = 0;
		}
	}
}
std::string HardcoreProtester::getItemType()
{
	return "Hardcore Protester";
}
void HardcoreProtester::bribe()
{
	m_stu_world->playSound(SOUND_PROTESTER_FOUND_GOLD);
	m_stu_world->increaseScore(50 /*points*/);
	m_ticksRested = -(100 - m_stu_world->getLevel() * 10);
	if (m_ticksRested <= -50)
		m_ticksRested = -50;
	
}