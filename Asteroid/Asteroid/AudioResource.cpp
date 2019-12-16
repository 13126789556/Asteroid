#include "AudioResource.h"
using namespace sf;

AudioResource::AudioResource(std::string resourceName) {
	buffer.loadFromFile(resourceName);
	sound.setBuffer(buffer);
	isActive = true;
}

void AudioResource::Play() {
	if (isActive) {
		sound.play();
	}
	else { sound.stop(); }
}
void AudioResource::PlayOnce() {
	if (isActive) {
		if (!isPlaying) { sound.play(); }
		isPlaying = true;
	}
	else {
		sound.stop();
		isPlaying = false;
	}
}