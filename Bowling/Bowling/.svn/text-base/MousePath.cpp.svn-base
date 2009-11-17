#include "MousePath.h"
#include <assert.h>

MousePath::MousePath()
:mPositions()
{
}

MousePath::~MousePath()
{
}

void MousePath::recordPosition(float x, float y)
{
	mPositions.push_back( Vec2D( x, y ) );
}

void MousePath::recordChangeInPosition( float x, float y )
{
	if( x == 0 || y == 0 )
		return;

	Vec2D lastPos = Vec2D(0,0);
	if( mPositions.size() != 0 )
		lastPos = mPositions[mPositions.size() - 1];

	lastPos.mX += x;
	lastPos.mY += y;

	mPositions.push_back( lastPos );
}

void MousePath::setPosition(int frame, float x, float y)
{
	assert( frame < mPositions.size() );
	mPositions[frame] = Vec2D( x, y );
}

void MousePath::resetPath()
{
	mPositions.clear();
}

Vec2D MousePath::getLeastSquaresLine(float &residual, bool &lastValuePosition)
{
	return Vec2D( -1, -1 );
}

Vec2D MousePath::getFirstHalf()
{
	if( mPositions.size() == 0 )
		return Vec2D(0,0);

	Vec2D start = mPositions[0];
	Vec2D end = mPositions[mPositions.size()/2];

	return Vec2D( start.mX - end.mX, start.mY - end.mY );
}

Vec2D MousePath::getSecondHalf()
{
	if( mPositions.size() == 0 )
		return Vec2D(0,0);
	Vec2D start = mPositions[mPositions.size()/2];
	Vec2D end = mPositions[mPositions.size()-1];

	return Vec2D( start.mX - end.mX, start.mY - end.mY );
}

float MousePath::getAngleDifference()
{
	Vec2D first = getFirstHalf();
	Vec2D second = getSecondHalf();

	return first.mX * second.mX + first.mY * second.mY;
}

int MousePath::getNumPositions()
{
	return mPositions.size();
}