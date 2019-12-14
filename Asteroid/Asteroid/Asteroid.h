#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
using namespace sf;
class Asteroid
{
public:
	Vector2f position, direction;
	float speed, radius;
	Sprite sprite;

	Asteroid(Vector2f position, float radius, float speed);
	void Move();
	void Warp();
	void Draw();
};

