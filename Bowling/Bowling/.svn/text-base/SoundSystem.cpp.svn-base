#include "SoundSystem.h"
#include "fmod_errors.h"
#include <stdio.h>
#include <windows.h>

SoundSystem::SoundSystem(void) {
	Initialize();
}

SoundSystem::~SoundSystem(void) {
	result = contactSound->release();
	ERRCHECK(result);

	result = system->release();
    ERRCHECK(result);
}

void SoundSystem::ERRCHECK(FMOD_RESULT result) {
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void SoundSystem::Initialize() {
	result = FMOD::System_Create(&system);		// Create the main system object.
	ERRCHECK(result);

	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	ERRCHECK(result);

	result = system->createStream("../Data/Win32/explosion.wav", FMOD_DEFAULT, 0, &contactSound);
    ERRCHECK(result);

	result = system->createStream("../Data/Win32/Sounds/nothingButTheSand.mp3", FMOD_LOOP_NORMAL, 0, &backgroundMusic);
	ERRCHECK(result);

	result = system->playSound(FMOD_CHANNEL_FREE, backgroundMusic, false, 0);
	ERRCHECK(result);

	result = system->createStream("../Data/Win32/Sounds/BowlingAmbient.mp3", FMOD_LOOP_NORMAL, 0, &backgroundMusic);
	ERRCHECK(result);

	result = system->playSound(FMOD_CHANNEL_FREE, backgroundMusic, false, 0);
	ERRCHECK(result);
}

void SoundSystem::PlayContactSound() {
	result = system->playSound(FMOD_CHANNEL_FREE, contactSound, false, 0);
    ERRCHECK(result);
}

void SoundSystem::Update() {
	system->update();
}