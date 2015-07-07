#include<iostream>
#include<ctime>
#include<vector>
#include"settings.h"
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

#pragma once

Plane::Plane()
{
	sf::Sprite plane_triangle;
	plane = plane_triangle;
	plane.setPosition(position_x, position_y);
	field = NULL;
	usable = false;
	fireStatus = 0;
	score = 0;
}

void Plane::sPosition(float x, float y)
{
	if (usable == false){
		std::cout << "object Plane is not usable" << std::endl;
		return;
	}
	plane.setPosition((float)x, (float)y);
}

void Plane::move(int direction)
{
	int offset_x=0;
	int offset_y=0;
	if (direction == UP){
		offset_y -= moveDistance;
	}
	else if (direction == DOWN)
	{
		offset_y += moveDistance;
	}
	else if (direction==RIGHT)
	{
		offset_x += moveDistance;
	}
	else if (direction == LEFT)
	{
		offset_x -= moveDistance;
	}
	if (usable == false){
		std::cout << "object Plane is not usable" << std::endl;
		return;
	}
	position_x += offset_x;
	position_y += offset_y;
	if (type == 1){
		if (!(position_x <= field->right_down.x - 160 && position_x >= field->left_down.x))
		{
			position_x -= offset_x;
		}
		if (!(position_y <= field->right_down.y&&position_y >= field->left_up.y))
		{
			position_y -= offset_y;
		}
	}

	plane.setPosition(position_x, position_y);
}

void Plane::addToDistrict(District* Field)
{
	if (Field == NULL){
		std::cout << "Field is NULL , add failed" << std::endl;
		return;
	}
	field = Field;
}

perShoot::perShoot()
{
	sf::Sprite Dot;
	sf::Texture Shoottexture;
	if (!Shoottexture.loadFromFile("1.png")) 
	{
		std::cout << "load texture plane_trangle failed" << std::endl;
	}
	texture = Shoottexture;
	Dot.setTexture(texture);
	pos_x = 20;
	pos_y = 700;
	Dot.setPosition(pos_x, pos_y);
	dot = Dot;
	Damage = 1;
}

void Plane::fire()
{
	fireStatus = 1;
}

void Plane::fireStop()
{
	fireStatus = 0;
}

void Shoot::update(sf::RenderWindow* pWindow,District* pField)
{
	std::vector<perShoot>::iterator searchElem;
	for (searchElem = ShootQueue.begin(); searchElem != ShootQueue.end();)
	{
		searchElem->pos_y += searchElem->moveDirection;
		bool outCheck = isOutofField(searchElem->pos_x, searchElem->pos_y, pField);
		if (outCheck){
			std::cout << "Shoot destroy" << std::endl;
			searchElem = ShootQueue.erase(searchElem);
		}
		else if (CollisionPlaneDamage(&(searchElem->dot),pField,searchElem->Damage,searchElem->moveDirection)) 
		{
			std::cout << "Shoot destroy" << std::endl;
			searchElem = ShootQueue.erase(searchElem);
		}
		else{
			searchElem->dot.setPosition(searchElem->pos_x, searchElem->pos_y);
			searchElem->update(pWindow);
			bool endCheck = (searchElem == ShootQueue.end());
			if (endCheck){
				break;
			}
			else{
				searchElem++;
			}
		}
		if (ShootQueue.empty()){
			break;
		}
	}

}

void perShoot::update(sf::RenderWindow* pWindow)
{

	pWindow->draw(dot);
}

void Shoot::throwShoot(float start_x, float start_y,pPlane target_plane,int combination=0)
{

	float timeInterval = 1;
	if (target_plane->type==1) 
	{
		timeInterval = target_plane->ShootTimeInterval;
	}
	else if(target_plane->type==2)
	{
		timeInterval = target_plane->ShootTimeInterval;
	}
	if (target_plane->ShootTmpTime.getElapsedTime().asSeconds() > timeInterval)
	{
		perShoot insertTmp;
		if (target_plane->type == 1) 
		{
			insertTmp.moveDirection = -5;
		}
		else if (target_plane->type == 2) 
		{
			insertTmp.moveDirection = 2;
		}
		insertTmp.pos_x = start_x;
		insertTmp.pos_y = start_y;
		ShootQueue.push_back(insertTmp);
		if (combination == 0) 
		{
			target_plane->ShootTmpTime.restart();
		}
	}
	else{
	}
}

Shoot::Shoot()
{

}

bool isOutofField(float target_x, float target_y, District* pField)
{
	if (target_x<=pField->right_down.x&&target_x>=pField->left_down.x)
	{
		if (target_y >= pField->right_up.y&&target_y <= pField->right_down.y)
			return false;
	}
	return true;
}

void District::ShootUpdate(sf::RenderWindow* pWindow, District* pField)
{
	int queueRotTemp = 0;
	for (; queueRotTemp < 10; queueRotTemp++){
		if (pPlaneQueue[queueRotTemp] != NULL){
			if (pPlaneQueue[queueRotTemp]->fireStatus == 1){
				float Shoot_x = pPlaneQueue[queueRotTemp]->position_x;
				float Shoot_y = pPlaneQueue[queueRotTemp]->position_y;
				if (queueRotTemp == 0) {
					ShootDistrict.throwShoot(Shoot_x + 90, Shoot_y, pPlaneQueue[queueRotTemp], 1);
				}
				ShootDistrict.throwShoot(Shoot_x, Shoot_y, pPlaneQueue[queueRotTemp]);

			}
			else{
				pPlaneQueue[queueRotTemp]->ShootTmpTime.restart();
			}

		}
	}
	ShootDistrict.update(pWindow, pField);
}

void District::PlaneAdd(pPlane pObject){
	int rouTmp = 0;
	if (usageCount < 10){
		for (rouTmp = 0; rouTmp < 10; rouTmp++){
			if (pPlaneQueue[rouTmp] == NULL){
				pPlaneQueue[rouTmp] = pObject;
				pObject->object_id = rouTmp;
				break;
			}
		}
		usageCount++;
	}
}

void Plane::setPlane(int type){
	if (type == 1){//player plane
		plane.setPosition(170, 700);
		this->type = 1;
		position_x = 170;
		position_y = 700;
		moveDistance = 1;
		Life = 30;
		ShootTimeInterval = 0.2;
		sf::Texture plane_triangle_texture;
		if (!plane_triangle_texture.loadFromFile("feiji.png")) {
			std::cout << "load texture plane_trangle failed" << std::endl;
		}
		texture = plane_triangle_texture;
		plane.setTexture(texture);
		if (field == NULL){
			usable = false;
		}
		else{
			usable = true;
		}
	}
	else if(type==2){
		position_x = 170; 
		position_y = 0;
		this->type = 2;
		ShootTimeInterval = 2;
		moveDistance = field->botSpeed;
		plane.setPosition(position_x, position_y);
		sf::Texture plane_triangle_texture;
		if (!plane_triangle_texture.loadFromFile("dj.png")) {
			std::cout << "load texture plane_trangle failed" << std::endl;
		}
		texture = plane_triangle_texture;
		plane.setTexture(texture);
		if (field == NULL){
			usable = false;
		}
		else{
			usable = true;
		}
		Life = 3;
	}
}

bool isCollisionWithPlane(sf::FloatRect* pBound1, sf::FloatRect* pBound) {
	if (pBound1->intersects(*pBound)) {
		return true;
	}
	return false;
}

void District::PlaneUpdate(sf::RenderWindow* pWindow){
	int rouTmp;
	if (pPlaneQueue[0] != NULL) {
		if (pPlaneQueue[0]->moveXStatus == -1) {
			pPlaneQueue[0]->move(LEFT);
		}
		else if (pPlaneQueue[0]->moveXStatus == 1) {
			pPlaneQueue[0]->move(RIGHT);
		}
		if (pPlaneQueue[0]->moveYStatus == 1) {
			pPlaneQueue[0]->move(UP);
		}
		else if (pPlaneQueue[0]->moveYStatus == -1) {
			pPlaneQueue[0]->move(DOWN);
		}
	for (rouTmp = 0; rouTmp < 10; rouTmp++){
		if (pPlaneQueue[rouTmp] == NULL){

		}
		else{
			if (pPlaneQueue[rouTmp]->usable == 1){
				if (pPlaneQueue[rouTmp]->type == 2){
					bool rangeCheck = isOutofField(pPlaneQueue[rouTmp]->position_x, pPlaneQueue[rouTmp]->position_y, this);
					bool collisionCheck = isCollisionWithPlane(&(pPlaneQueue[rouTmp]->plane.getGlobalBounds()), &(pPlaneQueue[0]->plane.getGlobalBounds()));
					if (collisionCheck) {
						pPlaneQueue[rouTmp]->onDestroy();
						pPlaneQueue[0]->onDamage(10);
						std::cout << "Crash!" << std::endl;
						continue;
					}
					else if (rangeCheck){
						pPlaneQueue[rouTmp]->onDestroy();
						continue;
					}
					else{
						pPlaneQueue[rouTmp]->move(DOWN);
						pPlaneQueue[rouTmp]->fire();
					}
				}
					__try {
						pWindow->draw(pPlaneQueue[rouTmp]->plane);//nothing
					}
					__except (EXCEPTION_EXECUTE_HANDLER) {
						std::cout << "draw error" << std::endl;
					}
				}
			}
		}
	}
}

void Plane::onDestroy(){
	std::cout << "enemy Destroy!!" << std::endl;
	if (this->type == 1) {
		this->field->gameStatus = 0;
	}
	if (this->type == 2) {
		field->pPlaneQueue[this->object_id] = NULL;
		field->usageCount--;
	}

	sf::SoundBuffer background_audio_buffer;

}

bool CollisionPlaneDamage(sf::Sprite* pShoot, District* Field,int damage,int moveDirection) {
	sf::FloatRect ShootBounds = pShoot->getGlobalBounds();
	int queueRotTemp = 1;
	if (pShoot->getGlobalBounds().intersects(Field->pPlaneQueue[0]->plane.getGlobalBounds())) {
		if (moveDirection > 0) {
			std::cout << "You get hit" << std::endl;
			Field->pPlaneQueue[0]->onDamage(damage);
			return true;
		}
	}
	for (; queueRotTemp < 10; queueRotTemp++) {
		if (Field->pPlaneQueue[queueRotTemp] != NULL) {
			sf::FloatRect planeBounds = Field->pPlaneQueue[queueRotTemp]->plane.getGlobalBounds();
			if (ShootBounds.intersects(planeBounds)) {
				if(Field->pPlaneQueue[queueRotTemp]->type == 2 && moveDirection < 0) {
					std::cout << "Collision! Plane Destroy" << std::endl;
					Field->pPlaneQueue[0]->scoreAdd(2);
					Field->pPlaneQueue[queueRotTemp]->onDamage(damage);
					return true;
				}
			}
		}
	}
	return false;
}

void Plane::onDamage(int damage=1) {
	if (Life<=0) {
		onDestroy();
	}
	else {
		Life -= damage;
	}
}

void AppendBotPlane(District* pField) {
	while (1) {
		srand(pField->usageCount+rand());
		int timer = 0;
		while (1) {
			timer = rand();
			if (timer < pField->timeIntervalMax && timer>pField->timeIntervalMin) {
				break;
			}
		}
		Sleep(timer);
		if (pField->usageCount < 10) {
			Plane* bot = new Plane;
			bot->addToDistrict(pField);
			pField->PlaneAdd(bot);
			int loc_x = 0;
			while (1) {
				loc_x = rand();
				if (loc_x > 500) {
					loc_x = loc_x / 100;
				}
				if (loc_x < 500 && loc_x>10) {
					break;
				}
			}
			bot->setPlane(2);
			bot->position_x = loc_x;
		}
	}
}

DWORD WINAPI thread2(PVOID pvParam) {
	AppendBotPlane((District*)pvParam);
	return 1;
}

void Plane::scoreAdd(int scoreAdded) {
   	score += scoreAdded;
}

void Plane::moveXStart(int direction) {
	if (direction == RIGHT) {
		moveXStatus = 1;
	}
	else if (direction == LEFT) 
	{
		moveXStatus = -1;
	}
}

void Plane::moveXStop() {
	moveXStatus = 0;
}

void Plane::moveYStart(int direction) 
{
	if (direction == UP) {
		moveYStatus = 1;
	}
	else if (direction == DOWN) 
	{
		moveYStatus = -1;
	}
}

void Plane::moveYStop() 
{
	moveYStatus = 0;
}

void LevelSet(District* field) 
{
	while (field->gameStatus == 1) 
	{
		if (field->pPlaneQueue[0] != NULL) 
		{
			if (field->pPlaneQueue[0]->score < 10) 
			{
				field->Level = 1;
			}
			else if(field->pPlaneQueue[0]->score > 10&& field->pPlaneQueue[0]->score < 900)
			{
				field->Level = 2;
				field->timeIntervalMax = 1000;
				field->timeIntervalMin = 300;
				field->botSpeed = 2;
			}
		}
	}
}

DWORD WINAPI levelThread_f(PVOID pvParam) 
{
	LevelSet((District*)pvParam);
	return 1;
}