#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
using namespace sf;
class Bullet
{
public:
	Vector2f position, direction;
	float speed, radius, displacement;
	Sprite sprite;

	Bullet(Vector2f position, Vector2f direction);
	bool Move();
	void Warp();
	void Draw();
};

