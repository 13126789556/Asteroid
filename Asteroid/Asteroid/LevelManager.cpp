#define PI 3.141592653
#include "LevelManager.h"
#include "Asteroid.h"
#include "Ship.h"
#include "Bullet.h"
#include "AudioResource.h"

extern float Magnitude(Vector2f v);
extern Vector2f Normalize(Vector2f v);
extern float Dot(Vector2f v1, Vector2f v2);
extern Ship player;
extern std::vector<Bullet> bullets;
extern std::vector<Bullet>::iterator bulletsIt;
extern std::vector<Asteroid> asteroids;
extern std::vector<Asteroid>::iterator asteroidsIt;
extern std::vector<SpriteAnimation> explosions;
extern RenderWindow window;
extern SpriteAnimation explosion;
extern AudioResource destroy;

LevelManager::LevelManager() {
	levelRank = 0;
}

void LevelManager::NewLevel() {
	player.position = Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	player.velocity = Vector2f(0, 0);
	player.direction = Vector2f(0, -1);
	player.degree = 180 / PI * (atan2(player.direction.y, player.direction.x) - atan2(-1, 0));
	player.isActive = true;
	bullets.clear();
	asteroids.clear();
	srand(time(0));
	for (int i = 0; i < 2 + (int)(levelRank / 2); i++) {
		Asteroid asteroid = Asteroid(Vector2f(rand() % 1200, rand() % 900),
			70,
			200 + levelRank * 40);
		asteroid.speed += rand() % 100;
		if (Magnitude(asteroid.position - player.position) < asteroid.radius + player.radius) {
			asteroid.position = Vector2f(0, 0);
		}
		asteroid.direction = Normalize(asteroid.position - player.position);
		asteroids.push_back(asteroid);
	}
}

void LevelManager::CollisionDetection() {
	bool hitByBullet = false;
	asteroidsIt = asteroids.begin();
	Vector2f bltPos;
	while (asteroidsIt != asteroids.end()) {
		//asteroids and ship
		if (Magnitude((*asteroidsIt).position - player.position) <= (*asteroidsIt).radius + player.radius) {
			SpriteAnimation exp = explosion;
			exp.position = player.position;
			exp.color = Color(255, 130, 100, 255);
			explosions.push_back(exp);
			NewLevel();
			player.life--;
			destroy.Play();
			return;
		}
		//asteroids and bullets
		bulletsIt = bullets.begin();
		while (bulletsIt != bullets.end()) {
			if (Magnitude((*asteroidsIt).position - (*bulletsIt).position) <= (*asteroidsIt).radius + (*bulletsIt).radius) {
				bltPos = (*bulletsIt).position;
				bulletsIt = bullets.erase(bulletsIt);
				hitByBullet = true;
				SpriteAnimation exp = explosion;
				//exp.color = Color(250, 80, 80);
				exp.position = (*asteroidsIt).position;
				exp.sprite.setScale(Vector2f((*asteroidsIt).radius / 50, (*asteroidsIt).radius / 50));
				explosions.push_back(exp);
			}
			else { bulletsIt++; }
		}
		//asteroids and asteroids
		std::vector<Asteroid>::iterator asteroidsTempIt = asteroidsIt + 1;
		while (asteroidsTempIt != asteroids.end()) {
			if (Magnitude((*asteroidsIt).position - (*asteroidsTempIt).position) <= (*asteroidsIt).radius + (*asteroidsTempIt).radius) {
				//move one asteroid to another's edge
				(*asteroidsIt).position = (*asteroidsTempIt).position 
					+ Normalize((*asteroidsIt).position - (*asteroidsTempIt).position) 
					* ((*asteroidsTempIt).radius + (*asteroidsIt).radius);
				//change direction
				(*asteroidsIt).direction -= 2 * Dot((*asteroidsIt).direction, Normalize((*asteroidsIt).position - (*asteroidsTempIt).position)) * Normalize((*asteroidsIt).position - (*asteroidsTempIt).position);
				(*asteroidsTempIt).direction -= 2 * Dot((*asteroidsTempIt).direction, Normalize((*asteroidsTempIt).position - (*asteroidsIt).position)) * Normalize((*asteroidsTempIt).position - (*asteroidsIt).position);
			}
			asteroidsTempIt++;
		}

		if (hitByBullet) { 
			// split to small asteroids
			auto ast = *asteroidsIt;
			score++;
			asteroidsIt = asteroids.erase(asteroidsIt);
			destroy.Play();
			if (ast.radius - 25 > 0) {
				for (int i = 0; i < 2; i++) {
					Asteroid asteroid = Asteroid(ast.position + Vector2f(rand() % (2*(int)ast.radius) - (int)ast.radius, rand() % (2 * (int)ast.radius) - (int)ast.radius),
						ast.radius - 25,
						ast.speed + 50);
					asteroid.direction = Normalize(Vector2f(rand() % 2000 / 1000 - 1, rand() % 2000 / 1000 - 1));
					if (Magnitude(asteroid.direction) == 0) {
						asteroid.direction = Normalize(asteroid.position - bltPos);
					}
					asteroidsIt = asteroids.insert(asteroidsIt, asteroid);
				}
			}
		}
		else { asteroidsIt++; }
		hitByBullet = false;
	}
}