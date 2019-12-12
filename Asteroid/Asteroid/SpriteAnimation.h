#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class SpriteAnimation
{
public:
	Vector2f position;
	Vector2i  cellPos, cellSize;
	float speed, interval;
	int colNum, rowNum, frameNum, cellNO;
	Texture texture;
	Sprite sprite;
	Color color;
	IntRect cell;

	SpriteAnimation(std::string resourceName,
		int colNum,
		int rowNum,
		int frameNum);
	SpriteAnimation(std::string resourceName,
		int colNum,
		int rowNum,
		int frameNum,
		Vector2f position,
		Color color);
	void Update(float dt);
	void Draw(RenderWindow& win);
};

