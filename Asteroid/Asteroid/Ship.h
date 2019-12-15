#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include "SpriteAnimation.h"
#include "Bullet.h"
using namespace sf;
class Ship
{
public:
	Vector2f position, direction, velocity;
	float  maxSpeed, rotateSpeed, radius, fireCoolDown;
	int life;
	Sprite sprite;
	float degree;
	bool isActive;
	
	Ship(float radius);
	void Rotate(float rotateSpeed);
	void Accelerate(float acceleration);
	void Resistance();
	void Warp();
	void Fire();
	void Draw();
};

