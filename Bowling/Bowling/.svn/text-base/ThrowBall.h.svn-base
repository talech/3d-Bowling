#ifndef ThrowBall_H
#define ThrowBall_H

#include "GameState.h"
#include "MouseCurve.h"

class ThrowBall : public GameState
{
public:

	~ThrowBall();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static ThrowBall* getInstance() { return &mThrowBall; }

protected:

	MouseCurve mPath;
	static ThrowBall mThrowBall;

	ThrowBall() {}
};
#endif