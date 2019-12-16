#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
//#include "VectorCalculation.h"
#include "UI.h"
#include "AudioResource.h"
#include "SpriteAnimation.h"
#include "Ship.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "vector"
#include "LevelManager.h"
#include "PowerUp.h"
using namespace sf;

Vector2f winSize(1200,900);
int fps, frameCount;
enum GameStatus
{
	Menu, GamePlay, Win, GameOver
};
GameStatus gameStatus;
LevelManager levelManager;

//global variable
float Magnitude(Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}
float Dot(Vector2f v1, Vector2f v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
Vector2f Normalize(Vector2f v) {
	return Magnitude(v) == 0 ? Vector2f(0, 0) : (v / Magnitude(v));
}
Vector2f Vector2Rotate(Vector2f v, float radian) {
	return Vector2f(v.x * cos(radian) - v.y * sin(radian), v.x * sin(radian) + v.y * cos(radian));
}
Vector2f Lerp(Vector2f v1, Vector2f v2, float t) {
	if (t < 0) { t = 0; }
	if (t > 1) { t = 1; }
	return v1 * (1 - t) + v2 * t;
}
Texture shipTex, asteroidTex, bulletTex, trailTex;
RenderWindow window(VideoMode(winSize.x, winSize.y), "Asteroid!");
float deltaTime;
std::vector<Bullet> bullets;
std::vector<Bullet>::iterator bulletsIt;
std::vector<Bullet> enemyBullets;
std::vector<Bullet>::iterator enemyBulletsIt;
std::vector<Asteroid> asteroids;
std::vector<Asteroid>::iterator asteroidsIt;
std::vector<SpriteAnimation> explosions;
std::vector<SpriteAnimation>::iterator explosionsIt;
std::vector<PowerUp> powerUps;
std::vector<PowerUp>::iterator powerUpsIt;
Ship player(30);
Ship enemy(30);
SpriteAnimation explosion = SpriteAnimation("Explosion.png", 4, 4, 15);
SpriteAnimation trail("Trail.png", 6, 5, 30);
AudioResource fire("Fire.wav");
AudioResource destroy("Destroy.wav");
AudioResource thrusting("Thrusting.wav");

void Initial() {
	winSize = Vector2f(1200, 900);
	levelManager.score = 0;
}

int main()
{
	//load resources
	shipTex.loadFromFile("Ship.png");
	asteroidTex.loadFromFile("Asteroid.png");
	bulletTex.loadFromFile("Bullet.png");
	trailTex.loadFromFile("Trail.png");

	player = Ship(30);

	Initial();

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("Background_Texture.jpg");

	Sprite background;
	background.setTexture(backgroundTexture);
	background.setScale(winSize.x / backgroundTexture.getSize().x, winSize.y / backgroundTexture.getSize().y);

	UI fpsUI(20, Vector2f(winSize.x - 100, 0));
	UI titleUI(90, Vector2f(winSize.x / 2 - 250, 200));
	titleUI.content = "ASTEROID";
	UI scoreUI(40, Vector2f(winSize.x / 2 - 80, 0));
	scoreUI.content = "Score: " + std::to_string(levelManager.score);
	UI lifeUI(40, Vector2f(0, 0));
	lifeUI.content = "Life: " + std::to_string(player.life);
	UI gameOverUI(50, Vector2f(winSize.x / 2 - 280, winSize.y / 2 - 75));
	gameOverUI.content = "You Lose! \n\npress Z to continue";
	UI winUI(50, Vector2f(winSize.x / 2 - 280, winSize.y / 2 - 75));
	winUI.content = "You win! \n\npress Z to next level";
	UI menuUI(40, Vector2f(winSize.x / 2 - 220, winSize.y / 2 + 175));
	menuUI.content = "Press Z to start new game\n\nPress Esc to exit";
	
	float time = 0;
	Clock fpsClock, fpsUpdate;


	while (window.isOpen()){
		//deltatime and fps
		deltaTime = fpsClock.getElapsedTime().asSeconds();	//time between two frame
		time += deltaTime;
		frameCount++;
		if (frameCount >= 10) {	//cauculate fps per 10 frame
			fps = frameCount / time;
			//fpsUI.setString("fps:" + std::to_string(fps));
			frameCount = 0;
			time = 0;
		}
		if (fpsUpdate.getElapsedTime().asSeconds() >= 0.1) {	//update fps per 0.1s
			fpsUI.content = "fps:" + std::to_string(fps);
			fpsUpdate.restart();
		}
		fpsClock.restart();

		//status machine for gameplay logic
		switch (gameStatus) {
		case Menu:
			sf::Event menuEvent;
			while (window.pollEvent(menuEvent))			{
				if (menuEvent.type == Event::Closed || (menuEvent.type == sf::Event::KeyPressed) && (menuEvent.key.code == sf::Keyboard::Escape)){
					window.close();
				}
				if ((menuEvent.type == sf::Event::KeyPressed) && (menuEvent.key.code == sf::Keyboard::Z)) {
					levelManager.levelRank = 0;
					levelManager.NewLevel();
					player.isActive = true;
					gameStatus = GamePlay;
				}
			}
			break;
		case GamePlay:
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				gameStatus = Menu;
			}
			//ship rotate
			if (Keyboard::isKeyPressed(Keyboard::A)) { player.Rotate(-7); }
			else if (Keyboard::isKeyPressed(Keyboard::D)) { player.Rotate(7); }

			//ship move
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.Accelerate(550);
			}
			else if (Keyboard::isKeyPressed(Keyboard::S)) { player.Accelerate(-550); }
			else {
				player.Resistance();
			}
			player.Warp();

			//fire
			if (Keyboard::isKeyPressed(Keyboard::Space)) { player.Fire(); }
			
			//bullets move
			bulletsIt = bullets.begin();
			while(bulletsIt != bullets.end()){
				if (!(*bulletsIt).Move()) {
					bulletsIt = bullets.erase(bulletsIt);
				}
				else {
					(*bulletsIt).Warp();
					bulletsIt++;
				}
			}

			////animation play once 
			explosionsIt = explosions.begin();
			while (explosionsIt != explosions.end()) {
				if ((*explosionsIt).Play()) {
					explosionsIt++;
				}
				else { explosionsIt = explosions.erase(explosionsIt); }
			}

			//asteroids move
			asteroidsIt = asteroids.begin();
			while (asteroidsIt != asteroids.end()) {
				(*asteroidsIt).Move();
				(*asteroidsIt).Warp();
				asteroidsIt++;
			}

			levelManager.CollisionDetection();

			scoreUI.content = "Score: " + std::to_string(levelManager.score);
			lifeUI.content = "Life: " + std::to_string(player.life);
			//GOTO
			if (asteroids.size() == 0 && explosions.size() == 0) {
				player.isActive = false;
				gameStatus = Win;
			}
			else if (player.life <= 0) {
				player.isActive = false;
				if (explosions.size() == 0) {
					gameStatus = GameOver;
				}
			}
			break;
		case Win:
			if (Keyboard::isKeyPressed(Keyboard::Z)) {
				levelManager.levelRank++;
				player.life++;
				levelManager.NewLevel();
				player.isActive = true;
				gameStatus = GamePlay;
			}
			break;
		case GameOver:
			player.life = 3;
			levelManager.levelRank = 0;
			if (Keyboard::isKeyPressed(Keyboard::Z)) {
				scoreUI.content = "Score: " + std::to_string(levelManager.score = 0);
				gameStatus = Menu;
			}
			break;
		}

		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear(Color(0, 0, 0, 0));
		window.draw(background);
		
		//state machine for render 
		switch (gameStatus) {
		case Menu:
			//start menu
			titleUI.Draw();
			menuUI.Draw();
			break;
		case GamePlay:
			for (auto item : bullets) {
				item.Draw();
			}
			for (auto item : asteroids) {
				item.Draw();
			}
			player.Draw();
			for (auto item : powerUps) {
				item.Draw();
			}
			for (auto item : explosions) {
				item.Draw();
			}
			scoreUI.Draw();
			lifeUI.Draw();
			break;
		case Win:
			winUI.Draw();
			scoreUI.Draw();
			break;
		case GameOver:
			gameOverUI.Draw();
			scoreUI.Draw();
			break;
		}

		fpsUI.Draw();
		window.display();
	}

	return 0;
}
