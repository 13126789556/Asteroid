#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
using namespace sf;
class UI
{
public:
	Font font;
	Text text;
	float characterSize;
	Vector2f position;
	String content;

	UI(float characterSize, Vector2f position);
	void setText(String s);
	void Update();
	void Draw(RenderWindow& win);
};

