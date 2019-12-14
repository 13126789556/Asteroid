#define PI 3.141592653
#include "LevelManager.h"
#include "Asteroid.h"
#include "Ship.h"
#include "Bullet.h"

extern float Magnitude(Vector2f v);
extern Vector2f Normalize(Vector2f v);
extern float Dot(Vector2f v1, Vector2f v2);
extern Ship ship;
extern std::vector<Bullet> bullets;
extern std::vector<Bullet>::iterator bulletsIt;
extern std::vector<Asteroid> asteroids;
extern std::vector<Asteroid>::iterator asteroidsIt;
extern RenderWindow window;

LevelManager::LevelManager() {
	levelRank = 0;
}

void LevelManager::NewLevel() {
	ship.position = Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	ship.velocity = Vector2f(0, 0);
	ship.direction = Vector2f(0, -1);
	ship.degree = 180 / PI * (atan2(ship.direction.y, ship.direction.x) - atan2(-1, 0));
	bullets.clear();
	asteroids.clear();
	srand(time(0));
	for (int i = 0; i < 2 + (int)(levelRank / 2); i++) {
		Asteroid asteroid = Asteroid(Vector2f(rand() % 1200, rand() % 900),
			70,
			200 + levelRank * 40);
		asteroid.speed += rand() % 100;
		asteroid.direction = Normalize(asteroid.position - ship.position);
		asteroids.push_back(asteroid);
	}
}

void LevelManager::CollisionDetection() {
	bool hitByBullet = false;
	asteroidsIt = asteroids.begin();
	while (asteroidsIt != asteroids.end()) {
		//asteroids and ship
		if (Magnitude((*asteroidsIt).position - ship.position) <= (*asteroidsIt).radius + ship.radius) {
			NewLevel();
			ship.life--;
			return;
		}
		//asteroids and bullets
		bulletsIt = bullets.begin();
		while (bulletsIt != bullets.end()) {
			if (Magnitude((*asteroidsIt).position - (*bulletsIt).position) <= (*asteroidsIt).radius + (*bulletsIt).radius) {
				bulletsIt = bullets.erase(bulletsIt);
				hitByBullet = true;
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
			asteroidsIt = asteroids.erase(asteroidsIt);
			if (ast.radius - 25 > 0) {
				for (int i = 0; i < 2; i++) {
					Asteroid asteroid = Asteroid(ast.position + Vector2f(rand() % (2*(int)ast.radius) - (int)ast.radius, rand() % (2 * (int)ast.radius) - (int)ast.radius),
						ast.radius - 25,
						ast.speed + 80);
					asteroid.direction = Normalize(Vector2f(rand() % 2000 / 1000 - 1, rand() % 2000 / 1000 - 1));
					if (Magnitude(asteroid.direction) == 0) {
						asteroid.direction = Normalize(asteroid.position - ship.position);
					}
					asteroidsIt = asteroids.insert(asteroidsIt, asteroid);
				}
			}
		}
		else { asteroidsIt++; }
		hitByBullet = false;
	}
}