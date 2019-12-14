#include "Bullet.h"

extern Vector2f Normalize(Vector2f v);
extern Texture bulletTex;
extern float deltaTime;
extern RenderWindow window;

Bullet::Bullet(Vector2f position, Vector2f direction) {
	this->position = position;
	this->direction = Normalize(direction);
	radius = 5;
	sprite.setTexture(bulletTex);
	sprite.setOrigin(bulletTex.getSize().x / 2, bulletTex.getSize().y / 2);
	sprite.setScale(radius * 2 / bulletTex.getSize().x, radius * 2 / bulletTex.getSize().y);
	speed = 800;
	displacement = 0;
}

bool Bullet::Move() {
	displacement += speed * deltaTime;
	if (displacement > 1500) {
		return false;
	}
	else {
		position += direction * speed * deltaTime;
		return true;
	}
}

void Bullet::Warp() {
	if (position.x < -radius) { position.x = window.getSize().x + radius; }
	if (position.x > window.getSize().x + radius) { position.x = -radius; }
	if (position.y < -radius) { position.y = window.getSize().y + radius; }
	if (position.y > window.getSize().y + radius) { position.y = -radius; }
}

void Bullet::Draw() {
	sprite.setPosition(position);
	window.draw(sprite);
}