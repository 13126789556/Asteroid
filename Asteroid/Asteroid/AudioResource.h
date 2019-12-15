#pragma once
#include <SFML/Audio.hpp>
using namespace sf;

class AudioResource
{
public:
	SoundBuffer buffer;
	Sound sound;
	bool isActive;

	AudioResource(std::string resourceName);
	void Play();
};

