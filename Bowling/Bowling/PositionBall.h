#ifndef PositionBall_H
#define PositionBall_H

#include "GameState.h"

class PositionBall : public GameState
{
public:

	~PositionBall();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static PositionBall* getInstance() { return &mPositionBall; }

protected:

	static PositionBall mPositionBall;

	PositionBall() {}
};
#endif