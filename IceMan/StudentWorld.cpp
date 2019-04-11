#include "StudentWorld.h"

#include <string>
#include <time.h>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld() {
	m_player.reset();
	for (int x = 0; x < Max_X; ++x) //VERTICAL AXIS (X-Axis)
	{
		for (int y = 0; y < Max_Y; ++y)//HORIZONTAL AXIS (Y-Axis)
		{

			m_iceField[x][y].reset();

		}
	}
}
int StudentWorld::init()
{
	m_num_boulders = (getLevel() / 2) + 2;
	m_num_gold = 5 - (getLevel() / 2);
	m_num_oil = 2 + getLevel();


	int placed_boulders = 0;
	int placed_gold = 0;
	int placed_oil = 0;

	if (m_num_boulders > 9)
		m_num_boulders = 9;
	if (m_num_gold < 2)
		m_num_gold = 2;
	if (m_num_oil > 21)
		m_num_oil = 21;

	m_protester_elapsed_time = 0;
	m_protester_timer = 200 - getLevel();
	if (m_protester_timer < 25)
		m_protester_timer = 25;

	m_num_protesters = 2 + getLevel() * 1.5;
	if (m_num_protesters > 15)
		m_num_protesters = 15;

	m_chance_of_hardcore = getLevel() * 10 + 30;
	if (m_chance_of_hardcore > 90)
		m_chance_of_hardcore = 90;

	m_current_protesters = 0;
	m_player = make_unique<Iceman>(this, 30, 60);

	for (int x = 0; x < Max_X; x++) //VERTICAL AXIS (X-Axis)
	{
		for (int y = 0; y < Max_Y - 4; y++) //HORIZONTAL AXIS (Y-Axis)
		{
			m_iceField[x][y] = make_unique<Ice>(this, x, y); //"this" is the world we are currently in
			if ((x <= 33 && x >= 30) && (y > 4))
				m_iceField[x][y].reset();
			if(y>Max_Y-4)
				m_iceField[x][y].reset();
		}
	}
	//INSTANTIATE BOULDERS
	while (placed_boulders < m_num_boulders)
	{
		bool tooClose = false;
		int x = setBoulderX();
		int y = setBoulderY();

		if (m_ItemsAndProtesters.empty())
		{
			createBoulder(x, y);
			placed_boulders++;
			continue;
		}

		for (int i = 0; i < m_ItemsAndProtesters.size(); i++)
		{
			if (objectsAtDist(m_ItemsAndProtesters[i], x, y, 6))
			{
				tooClose = true;
			}
		}
		if (!tooClose)
		{
			createBoulder(x, y);
			placed_boulders++;
		}
		if (placed_boulders < m_num_boulders)
			continue;
		
	}
	//INSTANTIATE GOLD
	while (placed_gold < m_num_gold)
	{
		bool tooClose = false;
		int x = setPickupX();
		int y = setPickupY();

		if (m_ItemsAndProtesters.empty())
		{
			createNugget(x, y, true);
			placed_gold++;
			continue;
		}

		for (int i = 0; i < m_ItemsAndProtesters.size(); i++)
		{
			if (objectsAtDist(m_ItemsAndProtesters[i], x, y, 6))
			{
				tooClose = true;
			}
		}
		if (!tooClose)
		{
			createNugget(x, y, true);
			placed_gold++;
		}
		if (placed_gold < m_num_gold)
			continue;

	}
	//INSTANTIATE OIL
	while (placed_oil < m_num_oil)
	{
		bool tooClose = false;
		int x = setPickupX();
		int y = setPickupY();

		if (m_ItemsAndProtesters.empty())
		{
			createOil(x, y);
			placed_oil++;
			continue;
		}

		for (int i = 0; i < m_ItemsAndProtesters.size(); i++)
		{
			if (objectsAtDist(m_ItemsAndProtesters[i], x, y, 6))
			{
				tooClose = true;
			}
		}
		if (!tooClose)
		{
			createOil(x, y);
			placed_oil++;
		}
		if (placed_oil < m_num_oil)
			continue;

	}

	/*//PLACCEHOLDER TO SEE IF MY WALKABLE POINTS FUNCTION WORKS.  GET RID OF THIS AFTER CONFIRMATION
	int walkables[60][60];
	findWalkablePoints(walkables);
	for (int x = 0; x < 60; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			cout << walkables[x][y];
		}
		cout << endl;
	}*/

	possibleProtester(m_chance_of_hardcore);
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
	m_protester_elapsed_time++;
	if (m_player->getHP() > 0)
		m_player->doSomething();
	else if (m_player->getHP() <= 0)
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (m_num_oil == 0)
		return GWSTATUS_FINISHED_LEVEL;

	for (auto& i : m_ItemsAndProtesters) //rangebased for loop for our vector of assorted stuffs.
	{
		i->doSomething();
	}
	for (int k = 0; k < m_ItemsAndProtesters.size(); k++)
	{
		if (m_ItemsAndProtesters[k]->isActive() == false)
		{
			if (m_ItemsAndProtesters[k]->getItemType() == "Protester" || m_ItemsAndProtesters[k]->getItemType() == "Hardcore Protester")
				m_current_protesters--;
			m_ItemsAndProtesters[k].reset();
			m_ItemsAndProtesters.erase(m_ItemsAndProtesters.begin() + k);
			k--;
		}
		else
		{
			m_ItemsAndProtesters[k]->doSomething();
		}
	}
	possibleGoodie();
	if (m_protester_elapsed_time >= m_protester_timer && m_current_protesters <= m_num_protesters)
	{
		possibleProtester(m_chance_of_hardcore);
		m_protester_elapsed_time = 0;
		m_current_protesters++;
	}
	std::string statText = updateStatUI();
	setGameStatText(statText);



	//findWalkablePoints(m_walkablePoints);

	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	m_player.reset();
	for (int x = 0; x < Max_X; ++x) //VERTICAL AXIS (X-Axis)
	{
		for (int y = 0; y < Max_Y; ++y)//HORIZONTAL AXIS (Y-Axis)
		{
			m_iceField[x][y].reset();
		}
	}
	for (auto& i : m_ItemsAndProtesters)
		i.reset();
	m_ItemsAndProtesters.clear();
}

bool StudentWorld::containsIce(int xCoord, int yCoord)
{

	if (m_iceField[xCoord][yCoord])
		return true;
	else
		return false;
}

bool StudentWorld::containsBoulder(int xCoord, int yCoord)
{
	for (int i = 0; i < m_ItemsAndProtesters.size(); i++)
	{
		if (m_ItemsAndProtesters[i]->getItemType() == "Boulder")
		{

			for (int x = 0; x < 4; ++x) //VERTICAL AXIS (X-Axis)
			{
				for (int y = 0; y < 4; ++y)//HORIZONTAL AXIS (Y-Axis)
				{
					if (m_ItemsAndProtesters[i]->getX() + x == xCoord && m_ItemsAndProtesters[i]->getY() + y == yCoord)
						return true;
				}
			}
		}
	}
	return false;

}
bool StudentWorld::containsPlayer(int xCoord, int yCoord)
{
	if (m_player->getX() == xCoord && m_player->getY() == yCoord)
		return true;
	else
		return false;
}
bool StudentWorld::containsProtester(int xCoord, int yCoord, std::shared_ptr<Actor>& prot)
{
	for (int i = 0; i < m_ItemsAndProtesters.size(); i++)
	{
		if (m_ItemsAndProtesters[i]->getItemType() == "Protester" || m_ItemsAndProtesters[i]->getItemType() == "Hardcore Protester")
		{
			for (int x = 0; x < 4; ++x) //VERTICAL AXIS (X-Axis)
			{
				for (int y = 0; y < 4; ++y)//HORIZONTAL AXIS (Y-Axis)
				{
					if (m_ItemsAndProtesters[i]->getX() + x == xCoord && m_ItemsAndProtesters[i]->getY() + y == yCoord)
					{
						prot = m_ItemsAndProtesters[i];
						return true;
					}

				}
			}
		}
	}
	return false;
}
void StudentWorld::removeIce(int xCoord, int yCoord)
{	
	m_iceField[xCoord][yCoord].reset();
}
void StudentWorld::createSquirt(int xCoord, int yCoord, const GraphObject::Direction dir)
{
	m_ItemsAndProtesters.push_back(make_shared<Squirt>(this, xCoord, yCoord, dir));
}
void StudentWorld::createNugget(int xCoord, int yCoord, bool isPermanent)
{
	m_ItemsAndProtesters.push_back(make_shared<Nugget>(this, xCoord, yCoord, isPermanent));
}
void StudentWorld::createBoulder(int xCoord, int yCoord)
{
	m_ItemsAndProtesters.push_back(make_shared<Boulder>(this, xCoord, yCoord));
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (containsIce(xCoord + x, yCoord + y))
				removeIce(xCoord + x, yCoord + y);
		}
	}
}
void StudentWorld::createOil(int xCoord, int yCoord)
{
	m_ItemsAndProtesters.push_back(make_shared<Barrel>(this, xCoord, yCoord));
}
void StudentWorld::createSonar(int xCoord, int yCoord)
{
	m_ItemsAndProtesters.push_back(make_shared<Sonar>(this, xCoord, yCoord));
}
void StudentWorld::createPool(int xCoord, int yCoord)
{
	m_ItemsAndProtesters.push_back(make_shared<Pool>(this, xCoord, yCoord));
}
void StudentWorld::createProtester(int xCoord, int yCoord, bool hardcore)
{
	if (!hardcore)
		m_ItemsAndProtesters.push_back(make_shared<Protester>(this, xCoord, yCoord, IID_PROTESTER));
	else if (hardcore)
		m_ItemsAndProtesters.push_back(make_shared<HardcoreProtester>(this, xCoord, yCoord));
}
std::string StudentWorld::updateStatUI()
{
	std::string text;
	std::string sLevel, sLives, sHp, sWater, sGold, sOil, sSonar, sScore;
	int level, lives, hp, water, gold, oil, sonar, score;
	level = getLevel();
	lives = getLives();
	hp = m_player->getHP();
	water = m_player->getWater();
	gold = m_player->getGold();
	oil = getNumOil();
	sonar = m_player->getSonar();
	score = getScore();

	//-----FORMATTING FUN TIMES-----//

	if (level < 10)
		sLevel = to_string(level) + "  ";
	else
		sLevel = to_string(level) + "  ";

	sLives = to_string(lives) + "  ";

	if (hp == 10)
		sHp = "100%  ";
	else
		sHp = to_string(hp * 10) + "%  ";

	if (water < 10)
		sWater = " " + to_string(water) + "  ";
	else
		sWater = to_string(water) + "  ";

	if (gold < 10)
		sGold = " " + to_string(gold) + "  ";
	else
		sGold = to_string(gold) + "  ";

	if (oil < 10)
		sOil = " " + to_string(oil) + "  ";
	else
		sOil = to_string(oil) + "  ";

	if (sonar < 10)
		sSonar = " " + to_string(sonar) + "  ";
	else
		sSonar = to_string(sonar) + "  ";

	sScore = to_string(score);
	while (sScore.length() < 6)
		sScore = "0" + sScore;
	
	
	text = "Lvl: " + sLevel + "Lives: " + sLives + "Hlth: " + sHp + "Wtr: " 
		+ sWater + "Gld: " + sGold + "Oil Left: " + sOil + "Sonar: " + 
		sSonar + "Score: " + sScore;

	return text;
}
void StudentWorld::sonarReveal()
{
	for (auto& i : m_ItemsAndProtesters)
	{
		if (i->playerAtDist(12, i->getX(), i->getY()))
			i->setVisible(true);
	}
}
void StudentWorld::collectWater()
{
	m_player->addWater();
}
void StudentWorld::collectSonar()
{
	m_player->addSonar();
}
void StudentWorld::collectGold()
{
	m_player->addGold();
}
void StudentWorld::collectOil()
{
	m_num_oil--;
}
int StudentWorld::getNumOil()
{
	return m_num_oil;
}
int StudentWorld::setBoulderX()
{
	int coord = rand() % 60; //[0, 59]
	while (coord >= 26 && coord <= 34)
		coord = rand() % 60; //try again

	return coord;
}
int StudentWorld::setBoulderY()
{
	int coord = (rand() % 37) + 20; //[20, 56]
	return coord;
}
int StudentWorld::setPoolX()
{
	int coord = rand() % 60; //[0, 59]
	return coord;
}
int StudentWorld::setPoolY()
{
	int coord = rand() % 56; //[0, 55]
	return coord;
}
int StudentWorld::setPickupX()
{
	int coord = rand() % 60; //[0, 59]
	while (coord >= 26 && coord <= 34)
		coord = rand() % 60; //try again

	return coord;
}
int StudentWorld::setPickupY()
{
	int coord = rand() % 56; //[0, 55]
	return coord;
}
void StudentWorld::possibleGoodie()
{
	int G = (getLevel() * 25) + 300;
	int chance = rand() % G + 1;
	int x = setPoolX();
	int y = setPoolY();
	bool onIce = true;





	if (chance == 1)
	{
		if ((rand() % 5 + 1) == 1)
			createSonar(0, 60);

		while (onIce == true)
		{
			onIce = false;

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					if (containsIce(x + i, y + j))
					{
						onIce = true;
					}

				}
			}
			if (onIce)
			{
				x = setPoolX();
				y = setPoolY();
			}
			else
				createPool(x, y);
		}

	}
}
void StudentWorld::annoyPlayer(int d)
{
	m_player->annoy(d);
}
void StudentWorld::annoyProtesters(std::shared_ptr<Actor> prot, int d)
{
	for (auto& i : m_ItemsAndProtesters)
	{
		if (i->getItemType() == "Protester" || i->getItemType() == "Hardcore Protester")
		{
			if (i == prot)
				i->annoy(d);
		}
	}
}
void StudentWorld::bribeProtesters(std::shared_ptr<Actor> prot)
{
	for (auto& i : m_ItemsAndProtesters)
	{
		if (i->getItemType() == "Protester" || i->getItemType() == "Hardcore Protester")
		{
			if (i == prot)
				i->bribe();
		}
	}
}
bool StudentWorld::objectsAtDist(std::shared_ptr<Actor>& a, int i, int j, int d)
{
	int xDist = i - a->getX();
	int yDist = j - a->getY();

	int totalDist = sqrt(pow(xDist, 2) + pow(yDist, 2));
	
	if (totalDist <= d)
		return true;
	else
		return false;
}
int StudentWorld::getPlayerX()
{
	return m_player->getX();
}
int StudentWorld::getPlayerY()
{
	return m_player->getY();
}
bool StudentWorld::isWalkablePoint(int xCoord, int yCoord)
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			if (containsIce(xCoord + x, yCoord + y) || containsBoulder(xCoord + x, yCoord + y))
				return false;
		}
	}
	return true;
}

void StudentWorld::findWalkablePoints(int arr[60][60])
{
	for (int x = 0; x < 60; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (isWalkablePoint(x, y))
				arr[x][y] = 0; //walkable
			else
				arr[x][y] = -1; //unwalkable
		}
	}
}
void StudentWorld::possibleProtester(int chance)
{
	int prob = rand() % 100 + 1;
	if (prob <= chance)
		createProtester(60, 60, true);
	else
		createProtester(60, 60, false);
}
/*
point* StudentWorld::breadthFirstSearch(point* origin, point* target)
{
	queue<point*> q;
	point* guess;
	q.push(origin);

	while (!q.empty())
	{
		guess = q.front();
		q.pop();
		if (guess == nullptr)
			continue;
		if (guess == target)
			return guess;
		
		q.push(guess->left);
		q.push(guess->right);
		q.push(guess->up);
		q.push(guess->down);


	}
}

//POINT STRUCT
struct point
{
	int xCoord, yCoord, dist;
	bool visited = false;
	point *left, *right, *up, *down;
};

walkableTree::~walkableTree()
{
	destroy(origin);
}
void walkableTree::insert(point*& p, int x, int y, int d)
{
	if (p == nullptr) 
	{
		p = new point;
		p->xCoord = x;
		p->yCoord = y;
		p->dist = d;
		p->right = nullptr;
		p->left = nullptr;
		p->up = nullptr;
		p->down = nullptr;
		return;
	}
}
void walkableTree::destroy(point*& p)
{
	if (p != nullptr)
	{
		destroy(p->right);
		destroy(p->left);
		destroy(p->up);
		destroy(p->down);
	}
}*/
// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp