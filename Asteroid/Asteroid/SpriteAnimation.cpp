#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(std::string resourceName,
	int colNum,
	int  rowNum,
	int frameNum) {
	texture.loadFromFile(resourceName);
	cellPos = Vector2i(0, 0);
	cellSize = Vector2i(texture.getSize().x / colNum, texture.getSize().y / rowNum);
	cell = IntRect(cellPos, cellSize);
	interval = 0;
	cellNO = 0;
	this->speed = 1;
	this->colNum = colNum;
	this->rowNum = rowNum;
	this->frameNum = frameNum;
	this->position = Vector2f(cellSize.x / 2, cellSize.y / 2);
	this->color = Color(255, 255, 255);
	sprite.setTexture(texture);
	sprite.setOrigin(cellSize.x / 2, cellSize.y / 2);
	sprite.setColor(this->color);
	sprite.setPosition(this->position);
	sprite.setTextureRect(cell);
}

SpriteAnimation::SpriteAnimation(std::string resourceName,
	int colNum,
	int rowNum,
	int frameNum,
	Vector2f position,
	Color color) {
	texture.loadFromFile(resourceName);
	cellPos = Vector2i(0, 0);
	cellSize = Vector2i(texture.getSize().x / colNum, texture.getSize().y / rowNum);
	cell = IntRect(cellPos, cellSize);
	interval = 0;
	cellNO = 0;
	this->speed = 1;
	this->colNum = colNum;
	this->rowNum = rowNum;
	this->frameNum = frameNum;
	this->position = position;
	this->color = color;
	sprite.setTexture(texture);
	sprite.setOrigin(cellSize.x / 2, cellSize.y / 2);
	sprite.setColor(this->color);
	sprite.setPosition(this->position);
	sprite.setTextureRect(cell);
}

void SpriteAnimation::Update(float dt) {
	interval += dt * speed;
	float i = 1.0f / 30.0f;
	if (interval >= i) {
		interval = 0;
		cellNO++;
	}
	if (cellNO >= frameNum) {
		cellNO = 0;
	}
	cellPos.x = cellSize.x * (int)(cellNO % colNum);
	cellPos.y = cellSize.y * (int)(cellNO / colNum);
	cell = IntRect(cellPos, cellSize);
}

void SpriteAnimation::Draw(RenderWindow& win) {
	sprite.setPosition(position);
	sprite.setColor(color);
	cell = IntRect(cellPos, cellSize);
	sprite.setTextureRect(cell);
	win.draw(sprite);
}
