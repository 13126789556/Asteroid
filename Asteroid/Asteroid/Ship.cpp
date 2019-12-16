#define PI 3.141592653
#include "Ship.h"
#include "vector"
#include "AudioResource.h"
extern Texture shipTex;
extern float deltaTime;
extern RenderWindow window;
extern Vector2f Normalize(Vector2f v);
extern float Magnitude(Vector2f v);
extern Vector2f Vector2Rotate(Vector2f v, float radian);
extern std::vector<Bullet> bullets;
extern SpriteAnimation trail;
extern AudioResource fire;
extern AudioResource thrusting;

Ship::Ship(float radius) {
	this->radius = radius;
	position = Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	sprite.setTexture(shipTex);
	sprite.setOrigin(shipTex.getSize().x / 2, shipTex.getSize().y / 2);
	sprite.setScale(radius * 2 / shipTex.getSize().x, radius * 2 / shipTex.getSize().y);
	direction = Vector2f(0, -1);
	life = 3;
	degree = 0;
	fireCoolDown = 0;
	maxSpeed = 600;
	isActive = true;
	invincible = false;
	invincibleRemain = 5;
	spreadShotRemain = 5;
	rapidShotRemain = 5;
	spreadShot = false;
	rapidShot = false;
	trail.sprite.setScale(Vector2f(0.8, 0.8));
	trail.speed = 3;
}

void Ship::Rotate(float rotateSpeed) {
	if (isActive) {
		direction = Vector2Rotate(direction, deltaTime * rotateSpeed);
		direction = Normalize(direction);
		degree = 180 / PI * (atan2(direction.y, direction.x) - atan2(-1, 0));
		sprite.setRotation(degree > 0 ? degree : degree + 360);
	}
}

void Ship::Accelerate(float acceleration) {
	if (isActive) {
		velocity += deltaTime * acceleration * direction;
		if (Magnitude(velocity) > maxSpeed) { velocity = maxSpeed * Normalize(velocity); }
		position += deltaTime * velocity;
		if (acceleration > 0) {
			trail.sprite.setRotation(degree - 180);
			trail.position = position - direction * radius * 2.0f;
			trail.isActive = true;
			trail.Update();
			thrusting.isActive = true;
		}
	}
}

void Ship::Resistance() {
	if (isActive) {
		velocity -= deltaTime * 150 * Normalize(velocity);
		if (Magnitude(velocity) <= 5) {
			velocity = Vector2f(0, 0);
		}
		position += deltaTime * velocity;
		trail.isActive = false;
		thrusting.isActive = false;
	}
}

void Ship::Warp() {
	if (isActive) {
		if (position.x < -radius) { position.x = window.getSize().x + radius; }
		if (position.x > window.getSize().x + radius) { position.x = -radius; }
		if (position.y < -radius) { position.y = window.getSize().y + radius; }
		if (position.y > window.getSize().y + radius) { position.y = -radius; }
	}
}

void Ship::Fire() {
	if (isActive) {
		fireCoolDown -= deltaTime;
		if (fireCoolDown <= 0) {
			fireCoolDown = rapidShot ? 0.1 : 0.2;
			Bullet bullte = Bullet(position + direction * radius, direction);
			bullets.push_back(bullte);
			if (spreadShot) {
				for (int i = 0; i < 2; i++) {
					Bullet bullte = Bullet(position + direction * radius, Vector2Rotate(direction, i * 0.3 - 0.15));
					bullets.push_back(bullte);
				}
			}
			fire.Play();
		}
	}
}

void Ship::Draw() {
	if (isActive) {
		//invincible update
		if (invincible) {
			invincibleRemain -= deltaTime;
			sprite.setColor(Color(255, 0, 0));
			invincible = invincibleRemain <= 0 ? false : true;
		}
		else {
			sprite.setColor(Color(255, 255, 255));
			invincibleRemain = 5;
		}
		//spread shot update
		if (spreadShot) {
			spreadShotRemain -= deltaTime;
			spreadShot = spreadShotRemain<= 0 ? false : true;
		}
		else {
			spreadShotRemain = 5;
		}
		//rapid shot update
		if (rapidShot) {
			rapidShotRemain -= deltaTime;
			rapidShot = rapidShotRemain <= 0 ? false : true;
		}
		else {
			rapidShotRemain = 5;
		}
		sprite.setPosition(position);
		sprite.setRotation(degree);
		trail.Draw();
		window.draw(sprite);
		thrusting.PlayOnce();
	}
}