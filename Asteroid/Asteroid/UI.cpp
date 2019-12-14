#pragma once
#include "UI.h"
using namespace sf;

extern RenderWindow window;

UI::UI(float characterSize, Vector2f position) {
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	this->characterSize = characterSize;
	this->position = position;
}

void UI::Update() {

}

void UI::setText(String s) {
	this->content = s;
}

void UI::Draw() {
	//text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setPosition(position);
	text.setString(content);
	window.draw(text);
}