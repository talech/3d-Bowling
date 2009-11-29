#include "MouseCurve.h"
#include <math.h>


MouseCurve::MouseCurve()
:mPositions()
,mTurnAroundPoint(0)
,mFramesElapsed(0)
,mIsGoingForward(false)
{
	mPositions.push_back(Vec2(0,0));
}

MouseCurve::~MouseCurve()
{
}

void MouseCurve::recordChangeInPosition(int delX, int delY)
{
	if( mIsGoingForward )
	{
		mFramesElapsed++;
	}

	if( delY == 0 )
		return;

	Vec2 oldPos = mPositions[mPositions.size()-1];

	// if this is before the change in direction and the mouse is moving forward
	// the direction has changed
	if( delY > 0 )
	{
		mTurnAroundPoint = mPositions.size();
	}
	else
	{
		mIsGoingForward = true;
	}


	mPositions.push_back( Vec2( oldPos.x + delX, oldPos.y + delY ) );
}

void MouseCurve::resetCurve()
{
	mPositions.clear();
	mPositions.push_back(Vec2(0,0));
	mTurnAroundPoint = 0;
	mIsGoingForward = false;
}

Vec2 MouseCurve::getBackPath()
{
	if( mPositions.size() == 1 )
		return Vec2(0,0);

	//Since the first slot is guaranteed to be (0,0), it doesn't make sense 
	//to subtract anything.
	return mPositions[mTurnAroundPoint];
}

float MouseCurve::getSpeedForward()
{
	if( !mIsGoingForward )
		return 0;

	Vec2 backPos = mPositions[mTurnAroundPoint];
	Vec2 forwardPos = mPositions[mPositions.size()-1];
	Vec2 forwardVector = Vec2( forwardPos.x - backPos.x, forwardPos.y - backPos.y );

	float distanceTraveled = sqrt( pow(forwardVector.x,2.0) + pow(forwardVector.y,2.0) );

	return distanceTraveled / mFramesElapsed;
}

float MouseCurve::getAngleDifference()
{
	Vec2 backPos = mPositions[mTurnAroundPoint];
	Vec2 forwardPos = mPositions[mPositions.size()-1];

	float backDist = sqrt( pow( backPos.x, 2.0 ) + pow( backPos.y, 2.0 ) );
	float forwardDist = sqrt( pow( forwardPos.x, 2.0 ) + pow( forwardPos.y, 2.0 ) );

	float dot = -backPos.x * forwardPos.x - backPos.y * forwardPos.y;

	if( backDist == 0 || forwardDist == 0 )
		return 0;

	return acos( dot/(backDist * forwardDist) );
}