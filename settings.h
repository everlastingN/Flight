#include<iostream>
#include<vector>
#include<windows.h>
#include <SFML/Graphics.hpp>
#ifndef SETTINGS_H
#define SETTINGS_H
#define UP 4
#define DOWN 8
#define LEFT 1
#define RIGHT 2

class District;
class Plane;
class Shoot;
class perShoot;

typedef Plane* pPlane;

class perShoot:public sf::Transformable{
public:
	perShoot();
	void update(sf::RenderWindow*);

	sf::Sprite dot;
	sf::Texture texture;
	int pos_x;
	int pos_y;
	int Damage;
	int moveDirection;
};

class Shoot{
public:
	Shoot();
	void update(sf::RenderWindow*,District*);
	void throwShoot(float, float, pPlane,int);
	std::vector<perShoot> ShootQueue;
};

class Plane:public sf::Transformable{
public:
	int object_id;
	int type;
	int moveXStatus;
	int moveYStatus;
	sf::Sprite plane;
	float position_x;
	float position_y;
	float ShootTimeInterval;
	int moveDistance;
	District* field;
	bool usable;
	sf::Clock runningTime;
	sf::Clock ShootTmpTime;
	int Life;
	int score;
	sf::Texture texture;

	Plane();
	void addToDistrict(District*);
	void sPosition(float, float);
	void move(int);
	void fire();
	void fireStop();
	int fireStatus;
	void onDamage(int);
	void onDestroy();
	void setPlane(int);
	void scoreAdd(int);
	void moveXStart(int);
	void moveYStart(int);
	void moveXStop();
	void moveYStop();

};

class District{
public:
	int Level;
	sf::Vector2<float> left_up, left_down, right_up, right_down;
	int maxPlane, usageCount;
	pPlane pPlaneQueue[10];
	int gameStatus;
	int timeIntervalMin, timeIntervalMax;
	int botSpeed;

	Shoot ShootDistrict;

	District::District(){
		timeIntervalMax = 4000;
		timeIntervalMin = 600;
		botSpeed = 1;
		Level = 1;
		left_down.x = 0;
		left_down.y = 900;
		left_up.x = 0;
		left_up.y = 0;
		right_up.x = 500;
		right_up.y = 0;
		right_down.x = 500;
		right_down.y = 900;
		maxPlane = 10;
		usageCount = 0;
		int temp = 0;
		gameStatus = 1;
		for (; temp < 10; temp++){
			pPlaneQueue[temp] = NULL;
		}
	}

	friend bool CollisionPlaneDamage(sf::Sprite*,District*,int,int);
	void ShootUpdate(sf::RenderWindow*, District*);
	void PlaneUpdate(sf::RenderWindow*);
	void PlaneAdd(pPlane);
	friend void AppendBotPlane(District*);
};

bool isOutofField(float, float, District*);

DWORD WINAPI thread2(PVOID);

DWORD WINAPI levelThread_f(PVOID);

void LevelSet(District*);

#endif 