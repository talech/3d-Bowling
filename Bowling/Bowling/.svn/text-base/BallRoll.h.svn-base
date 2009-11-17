#ifndef BallRoll_H
#define BallRoll_H

#include "GameState.h"

class BallRoll : public GameState
{
public:

	~BallRoll();

	void enter();
	void exit();

	void pause();
	void resume();

	void processMouse( Mouse* mouse );
	void processKeyboard( Keyboard* keyboard );
	void processGamePad( GamePad* gamepad );

	void update( float delTime );

	static BallRoll* getInstance() { return &mBallRoll; }

protected:

	static BallRoll mBallRoll;

	BallRoll() {}

	int count;
};
#endif