#ifndef MousePath_H
#define MousePath_H

#include <vector>
// A class used to record the path of the mouse

struct Vec2D
{
	float mX;
	float mY;

	Vec2D( float x, float y ) { mX = x; mY = y; }
};

class MousePath
{
public:

	MousePath();
	~MousePath();

	void recordPosition( float x, float y );
	void recordChangeInPosition( float x, float y );
	void setPosition( int frame, float x, float y );

	Vec2D getLeastSquaresLine( float& residual, bool& lastValuePosition );

	Vec2D getFirstHalf();
	Vec2D getSecondHalf();
	float getAngleDifference();

	int getNumPositions();

	void resetPath();

private:

	std::vector<Vec2D> mPositions;
};
#endif