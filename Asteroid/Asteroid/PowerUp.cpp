#include "PowerUp.h"
extern RenderWindow window;

PowerUp::PowerUp(Vector2f position) {
	this->position = position;
	type = SPREADSHOT;
	radius = 20;
	circle = CircleShape(radius, 6);
	circle.setOrigin(radius, radius);
}

void PowerUp::Draw() {
	circle.setPosition(position);
	switch (type)
	{
	case PowerUp::INVINCIBLE:
		circle.setFillColor(Color(255, 255, 125));
		break;
	case PowerUp::SPREADSHOT:
		circle.setFillColor(Color(255, 60, 90));
		break;
	case PowerUp::RAPIDSHOT:
		circle.setFillColor(Color(20, 110, 255));
		break;
	default:
		break;
	}
	window.draw(circle);
}