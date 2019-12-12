#include "AudioResource.h"
using namespace sf;

AudioResource::AudioResource(std::string resourceName) {
	buffer.loadFromFile(resourceName);
	sound.setBuffer(buffer);
}

void AudioResource::Play() {
	sound.play();
}

//void AudioResource::Play() {
//	sound.play();
//}
//
//void AudioResource::Play() {
//	sound.play();
//}