#ifndef MouseCurve_H
#define MouseCurve_H

#include <vector>

struct Vec2
{
	int x;
	int y;

	Vec2( int mX, int mY ){ x = mX; y = mY; }
};

class MouseCurve
{
public:

	MouseCurve();

	~MouseCurve();

	void resetCurve();

	void recordChangeInPosition( int delX, int delY );

	//Use this to set the direction of the ball
	//Returns the vector from the initial position to the point where the mouse changes direction.
	Vec2 getBackPath();

	//Use this to set the speed of the ball.
	//Returns the speed at which the mouse went forward in screen units/frame
	float getSpeedForward();

	//Use this to find the spin of the ball.
	float getAngleDifference();

private:
	std::vector<Vec2> mPositions;

	int mFramesElapsed;
	int mTurnAroundPoint;
	bool mIsGoingForward;


};
#endif