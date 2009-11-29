#include "ThrowBall.h"
#include "BallRoll.h"
#include <math.h>

ThrowBall ThrowBall::mThrowBall;

ThrowBall::~ThrowBall()
{
}

void ThrowBall::enter()
{
	//NiCamera* cam = (NiCamera*)GameStateManager::getInstance()->scene->GetObjectByName("camera1");
	//GameStateManager::getInstance()->setCamera(cam);
	mPath.resetCurve();

	
}

void ThrowBall::exit()
{

	NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();

	// New Hotness
	Vec2 direction = mPath.getBackPath();
	float speedScreenCoords = mPath.getSpeedForward();
	float spinAngle = mPath.getAngleDifference();
	
	// Flip the data around so the ball can go in the proper direction
	direction.x *= -1;
	direction.y *= -1;

	//Normalize the direction
	float dirLength = sqrt( pow(direction.x, 2.0) + pow(direction.y, 2.0) );
	if( dirLength == 0 )
		dirLength = 1;

	float dirX = direction.x / dirLength;
	float dirY = direction.y / dirLength;

	float ballImpulse = speedScreenCoords * 100;
	//Don't want to break land speed records
	//Or the game.
	//Which I just lost.
	if( ballImpulse > 2000 )
		ballImpulse = 2000;
	if( ballImpulse < 1000 )
		ballImpulse = 1000;
	//Scale the direction
	dirX *= ballImpulse;
	dirY *= ballImpulse;

	//Damp the x value
	dirX /= 3.0;
	//Add the force!
	ballActor->addForce( NxVec3(dirX, 0, dirY) );

	//Add the torque!
	ballActor->addTorque( NxVec3(0, spinAngle*speedScreenCoords, 0) );

	//Move the ball up!
	ballActor->setGlobalPosition( ballActor->getGlobalPosition() + NxVec3(0,1,0));
}

void ThrowBall::pause()
{
}

void ThrowBall::resume()
{
}

void ThrowBall::processMouse(Mouse *mouse)
{
	if( mouse )
	{
		if( mouse->ButtonIsDown(NiInputMouse::Button::NIM_LEFT) )
		{
			int delX;
			int delY;
			int delZ;
			if (mouse->GetPositionDelta(delX, delY, delZ))
				mPath.recordChangeInPosition(delX, delY);
		}
		else
		{
			GameStateManager::getInstance()->changeState( BallRoll::getInstance() );
		}
	}
}

void ThrowBall::processKeyboard(Keyboard *keyboard)
{
}

void ThrowBall::processGamePad(GamePad *gamepad)
{
}

void ThrowBall::update(float delTime)
{
}