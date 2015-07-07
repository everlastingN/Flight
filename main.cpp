#include<iostream>
#include<sstream>
#include<windows.h>
#include"settings.h"
#include"collision.h"
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>


int main()
{
    //field
	sf::RenderWindow window(sf::VideoMode(480, 800), "SFML",sf::Style::Close);
	District field;

    //text
	sf::Font font;
	if (!font.loadFromFile("game.ttf")) {
		std::cout << "error" << std::endl;
	}
	sf::Text LifeText;
	sf::Text ScoreText;
	sf::Text levelText;
	LifeText.setFont(font);
	ScoreText.setFont(font);
	levelText.setFont(font);
	std::ostringstream life;
	std::ostringstream score;
	std::ostringstream level;
	LifeText.setCharacterSize(24);
	LifeText.setColor(sf::Color::Black);
	//LifeText.setPosition(200, 200);
	ScoreText.setCharacterSize(24);
	ScoreText.setColor(sf::Color::Black);
	ScoreText.setPosition(LifeText.getPosition().x, LifeText.getPosition().y+30);
	levelText.setCharacterSize(24);
	levelText.setColor(sf::Color::Black);
	levelText.setPosition(LifeText.getPosition().x, LifeText.getPosition().y + 60);
    //shapes
	Plane mainPlane;
	mainPlane.addToDistrict(&field);
	field.PlaneAdd(&mainPlane);
	mainPlane.setPlane(1);

    //backgroud
	sf::Texture backgroud;
	if (!backgroud.loadFromFile("background.png"))
		std::cout << "error" << std::endl;
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroud);

    //audio
	sf::SoundBuffer background_audio_buffer;
	
    //start
	window.setFramerateLimit(60);
	HANDLE backThread = CreateThread(NULL, 0, &thread2, &field, NULL, NULL);
	HANDLE levelThread = CreateThread(NULL, 0, &levelThread_f, &field, NULL, NULL);
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			
			//move
			if (event.type == sf::Event::KeyPressed){
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
					mainPlane.moveXStart(LEFT);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
					mainPlane.moveXStart(RIGHT);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
					mainPlane.moveYStart(UP);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
					mainPlane.moveYStart(DOWN);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
					mainPlane.fire();
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Space) {
					mainPlane.fireStop();
				}
				if (event.key.code == sf::Keyboard::Right) {
					mainPlane.moveXStop();
				}
				if (event.key.code == sf::Keyboard::Left) {
					mainPlane.moveXStop();
				}
				if (event.key.code == sf::Keyboard::Up) {
					mainPlane.moveYStop();
				}
				if (event.key.code == sf::Keyboard::Down) {
					mainPlane.moveYStop();
				}
			}
		}
		window.clear();
		window.draw(backgroundSprite);
		if (field.gameStatus == 1) {
			field.shootUpdate(&window, &field);
			field.PlaneUpdate(&window);
			life.clear();
			life.str("");
			score.str("");
			score.clear();
			level.str("");
			level.clear();
			life << "Life:" << mainPlane.Life;
			LifeText.setString(life.str());
			score << "Score:" << mainPlane.score;
			ScoreText.setString(score.str());
			level << "Level:" << field.Level;
			levelText.setString(level.str());
			window.draw(levelText);
			window.draw(LifeText);
			window.draw(ScoreText);
		}
		else if (field.gameStatus == 0) {
			std::cout << "YOU ARE DIED!" << std::endl;
			sf::Sprite Over;
			sf::Texture OverTexture;
			OverTexture.loadFromFile("gameover.png");
			Over.setTexture(OverTexture);
			window.draw(Over);
		}
		window.display();
	}
	
	TerminateThread(backThread,1);
	TerminateThread(levelThread, 1);
	return 0;
}