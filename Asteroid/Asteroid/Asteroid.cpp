#include "Asteroid.h"

extern Vector2f Normalize(Vector2f v);
extern Texture asteroidTex;
extern float deltaTime;
extern RenderWindow window;

Asteroid::Asteroid(Vector2f position, float radius, float speed) {
	this->position = position;
	this->radius = radius;
	this->speed = speed;
	direction = Normalize(Vector2f(rand() % 200 / 200 - 2, rand() % 200 / 200 - 2));
	sprite.setTexture(asteroidTex);
	sprite.setOrigin(asteroidTex.getSize().x / 2, asteroidTex.getSize().y / 2);
	sprite.setScale(radius * 2 / asteroidTex.getSize().x, radius * 2 / asteroidTex.getSize().y);
}

void Asteroid::Move() {
	position += direction * speed * deltaTime;
}

void Asteroid::Warp() {
	if (position.x < -radius) { position.x = window.getSize().x + radius; }
	if (position.x > window.getSize().x + radius) { position.x = -radius; }
	if (position.y < -radius) { position.y = window.getSize().y + radius; }
	if (position.y > window.getSize().y + radius) { position.y = -radius; }
}

void Asteroid::Draw() {
	sprite.setPosition(position);
	window.draw(sprite);
}