#pragma once

#include "fmod.hpp"

class SoundSystem
{
public:
	SoundSystem(void);
	~SoundSystem(void);
	
	void PlayContactSound();
	void Update();

private:
	void ERRCHECK(FMOD_RESULT result);
	void Initialize();

	FMOD_RESULT result;
	FMOD::System *system;
	FMOD::Channel *channel;
	FMOD::Sound *contactSound;
};