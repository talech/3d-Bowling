#ifndef ResetPins_H
#define ResetPins_H

#include "GameState.h"

class ResetPins : public GameState
{
public:

	~ResetPins();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static ResetPins* getInstance() { return &mResetPins; }

protected:

	int mNumPinsDown;

	static ResetPins mResetPins;

	ResetPins() {}
};
#endif