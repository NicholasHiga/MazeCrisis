#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"
#include "ResourceManager.h"

// We need this so we can make a shared_ptr for the ResourceManager template.
class SoundWrapper
{
public:
	SoundWrapper(FMOD::Sound* _sound) : sound(_sound) {};
	FMOD::Sound* sound;
	FMOD::Channel* channel;
};

class AudioManager : public ResourceManager<SoundWrapper>
{
public:
	static AudioManager* getInstance();
	bool loadSound(const std::string &soundPath);
	bool loadStream(const std::string &streamPath); 
		// Stream related functions are used for music
	void playSound(const std::string &soundPath, float volume = 1.0f);

	// Music functions
	void playStream(const std::string &streamPath, float volume = 1.0f);
		// For music, no fadein.
	void playStreamFadeIn(const std::string &streamPath, 
		unsigned int fadeDurationMS, float volume = 1.0f); 
		// Start playing music with fadein
	void playNextStream(const std::string &streamPath, 
		unsigned int fadeDurationMS, float volume);
		// Fade duration is combination of both fade in and out.
	void fadeStreamIn(unsigned int fadeDurationMS, float endVolume);
	void fadeStreamOut(unsigned int fadeDurationMS); // In milliseconds
	void setStreamVolume(float volume);	// Currently playing song.

	void update();
	float getSoundLengthMS(const std::string &soundPath);

	void errorCheck();
	bool isStreamPlaying;

private:
	std::string currentlyPlayingStream;

	float fadeOutStartTime, fadeOutDuration, fadeInVolume;
	bool fadingOut;
	std::string nextStream;

	static AudioManager* myInstance;
	static FMOD::System *system;
	static FMOD_RESULT result;

	AudioManager();
	~AudioManager();
};