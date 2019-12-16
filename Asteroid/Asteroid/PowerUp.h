#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
using namespace sf;
class PowerUp
{
public:
	Vector2f position;
	float radius;
	enum PowerUpType { INVINCIBLE, SPREADSHOT, RAPIDSHOT };
	PowerUpType type;
	CircleShape circle;

	PowerUp(Vector2f position);
	void Draw();
};

