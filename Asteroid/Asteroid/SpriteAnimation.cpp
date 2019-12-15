#include "SpriteAnimation.h"

extern RenderWindow window;
extern float deltaTime;


SpriteAnimation::SpriteAnimation(){}
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
	isLoop = true;
	isActive = true;
}

SpriteAnimation::SpriteAnimation(Texture texture,
	int colNum,
	int rowNum,
	int frameNum) {
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
	isLoop = true;
	isActive = true;
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
	isLoop = true;
	isActive = true;
}

void SpriteAnimation::Update() {
	interval += deltaTime * speed;
	float i = 1.0f / 30.0f;
	if (interval >= i) {
		interval = 0;
		cellNO++;
	}
	if (cellNO >= frameNum) {
		if (isLoop) {
			cellNO = 0;
		}
		else {
			cellNO = frameNum;
		}
	}
	cellPos.x = cellSize.x * (int)(cellNO % colNum);
	cellPos.y = cellSize.y * (int)(cellNO / colNum);
	cell = IntRect(cellPos, cellSize);
}

bool SpriteAnimation::Play() {
	interval += deltaTime * speed;
	float i = 1.0f / 30.0f;
	if (interval >= i) {
		interval = 0;
		cellNO++;
	}
	if (cellNO >= frameNum) {
		cellNO = frameNum;
	}
	cellPos.x = cellSize.x * (int)(cellNO % colNum);
	cellPos.y = cellSize.y * (int)(cellNO / colNum);
	cell = IntRect(cellPos, cellSize);
	if (cellNO >= frameNum) { return false; }
}

void SpriteAnimation::Draw() {
	if(isActive){
		sprite.setPosition(position);
		sprite.setColor(color);
		cell = IntRect(cellPos, cellSize);
		sprite.setTextureRect(cell);
		window.draw(sprite);
	}
}
