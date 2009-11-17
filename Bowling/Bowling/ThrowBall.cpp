#include "ThrowBall.h"
#include "BallRoll.h"

ThrowBall ThrowBall::mThrowBall;

ThrowBall::~ThrowBall()
{
}

void ThrowBall::enter()
{
	//NiCamera* cam = (NiCamera*)GameStateManager::getInstance()->scene->GetObjectByName("camera1");
	//GameStateManager::getInstance()->setCamera(cam);
	mPath.resetPath();

	
}

void ThrowBall::exit()
{

	NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();

	Vec2D direction = mPath.getFirstHalf();
	float spin = mPath.getAngleDifference();

	float forceScale = -100.0/mPath.getNumPositions();
	float spinScale = 10;

	spin *= spinScale;
	direction.mX *= forceScale/3.0;
	direction.mY *= forceScale;

	ballActor->setGlobalPosition( ballActor->getGlobalPosition() + NxVec3(0,1,0));
	ballActor->addTorque( NxVec3(0, spin, 0) );
	ballActor->addForce( NxVec3(direction.mX, 0, direction.mY) );
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