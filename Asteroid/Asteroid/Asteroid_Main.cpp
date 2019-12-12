#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
//#include "VectorCalculation.h"
#include "UI.h"
#include "AudioResource.h"
#include "SpriteAnimation.h"
using namespace sf;

Vector2f winSize(750, 900);
Vector2f p1Size, p2Size, obSize, p1Pos, p2Pos, obPos, bhPos;
Vector2f ballDir, ballPos;
float ballRadius, ballSpeed, p1Speed, p2Speed, t, obSpeed, bhRadius;
int fps, frameCount, score1, score2;
bool isStartMenu;
bool isAIMode;
bool isTestMode;

float Magnitude(Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

Vector2f Normalize(Vector2f v) {
	return v / Magnitude(v);
}

Vector2f Lerp(Vector2f v1, Vector2f v2, float t) {
	if (t < 0) { t = 0; }
	if (t > 1) { t = 1; }
	return v1 * (1 - t) + v2 * t;
}


void Initial() {
	winSize = Vector2f(750, 900);
	p1Size = Vector2f(160, 25);
	p2Size = Vector2f(160, 25);
	obSize = Vector2f(130, 40);
	p1Pos = Vector2f(winSize.x / 2, p1Size.y * 1.5);
	p2Pos = Vector2f(winSize.x / 2, winSize.y - p1Size.y * 1.5);
	bhPos = Vector2f(winSize.x / 2 + 200, winSize.y / 2 + 200);
	obPos = Vector2f(obSize.x / 2, winSize.y / 2);
	ballPos = Vector2f(winSize.x / 2, winSize.y / 2);
	ballDir = Normalize(Vector2f(0, 1));
	ballSpeed = 550;
	p1Speed = 600;
	p2Speed = 600;
	obSpeed = 350;
	ballRadius = 25.0f;
	bhRadius = 100;
	score1 = 0;
	score2 = 0;
	isStartMenu = true;
	isAIMode = true;
	isTestMode = false;
}

int main()
{
	Initial();
	RenderWindow window(VideoMode(winSize.x, winSize.y), "Pong!");

	SpriteAnimation anim("Sprite_Sheet_Test.png", 8, 1, 8);
	anim.speed = 0.3;
	SpriteAnimation kojima("kojima.png", 3, 3, 8);
	kojima.position = Vector2f(winSize.x / 2, winSize.y / 2);
	SpriteAnimation blackhole("blackhole.png", 5, 5, 24);
	blackhole.position = bhPos;

	AudioResource hit("Hit.wav");

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("Background_Texture.png");

	Sprite background;
	background.setTexture(backgroundTexture);

	UI fpsUI(20, Vector2f(winSize.x - 100, 0));
	UI score1UI(40, Vector2f(0, winSize.y / 2 - 60));
	score1UI.content = std::to_string(score1);
	UI score2UI(40, Vector2f(0, winSize.y / 2 + 10));
	score2UI.content = std::to_string(score2);
	UI winUI(50, Vector2f(winSize.x / 2 - 280, winSize.y / 2 - 75));
	UI menuUI(40, Vector2f(90, winSize.y / 2 - 75));
	menuUI.content = "Press a to play with AI\n\nPress b to play with human player\n\nPress t to test continuous";

	Time time, deltaTime;
	Clock fpsClock, fpsUpdate;

	while (window.isOpen())
	{
		//deltatime and fps
		deltaTime = fpsClock.getElapsedTime();	//time between two frame
		time += deltaTime;
		frameCount++;
		if (frameCount >= 10) {	//cauculate fps per 10 frame
			fps = frameCount / time.asSeconds();
			//fpsUI.setString("fps:" + std::to_string(fps));
			frameCount = 0;
			time = Time().Zero;
		}
		if (fpsUpdate.getElapsedTime().asSeconds() >= 0.1) {	//update fps per 0.1s
			fpsUI.content = "fps:" + std::to_string(fps);
			fpsUpdate.restart();
		}
		fpsClock.restart();

		//start menu
		if (isStartMenu) {
			deltaTime = Time().Zero;
			if (Keyboard::isKeyPressed(Keyboard::A) && deltaTime == Time().Zero) {
				isStartMenu = false;
			}
			else if (Keyboard::isKeyPressed(Keyboard::B) && deltaTime == Time().Zero) {
				isStartMenu = false;
			}
			else if (Keyboard::isKeyPressed(Keyboard::T) && deltaTime == Time().Zero) {
				ballSpeed = 10000;
				isStartMenu = false;
			}
		}

		//if win 
		if (score1 >= 5) {	//win detection
			deltaTime = Time().Zero;
			winUI.content = "			You Lose! \n\n press space to continue";
		}
		if (score2 >= 5) {	//win detection
			deltaTime = Time().Zero;
			winUI.content = "			You Win! \n\n press space to continue";
		}
		if (Keyboard::isKeyPressed(Keyboard::Space) && deltaTime == Time().Zero) {
			//score1 = score2 = 0;
			score1UI.content = std::to_string(score1 = 0);
			score2UI.content = std::to_string(score2 = 0);
			isStartMenu = true;
		}

		//ball collision detection



		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}





		//test
		anim.Update(deltaTime.asSeconds());
		kojima.Update(deltaTime.asSeconds());
		blackhole.Update(deltaTime.asSeconds());

		window.clear(Color(0, 0, 0, 0));
		//gameobject
		window.draw(background);
		blackhole.Draw(window);

		//test
		//anim.Draw(window);
		//kojima.Draw(window);

		//ui
		fpsUI.Draw(window);
		score1UI.Draw(window);
		score2UI.Draw(window);
		if (score1 >= 5 || score2 >= 5) {
			winUI.Draw(window);
		}
		if (isStartMenu) {
			menuUI.Draw(window);
		}
		window.display();
	}

	return 0;
}
