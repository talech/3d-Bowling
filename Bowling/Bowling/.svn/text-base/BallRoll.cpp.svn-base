#include "BallRoll.h"
#include "ResetPins.h"

BallRoll BallRoll::mBallRoll;

BallRoll::~BallRoll()
{
}

void BallRoll::enter()
{
	
	//NiCamera* cam = (NiCamera*)GameStateManager::getInstance()->scene->GetObjectByName("camera3");
	//GameStateManager::getInstance()->setCamera(cam);
	count = 0;
}

void BallRoll::exit()
{
}

void BallRoll::pause()
{
}

void BallRoll::resume()
{
}

void BallRoll::processMouse(Mouse *mouse)
{
}

void BallRoll::processKeyboard(Keyboard *keyboard)
{
}

void BallRoll::processGamePad(GamePad *gamepad)
{
}

void BallRoll::update(float delTime)
{
	
	// A call to Simulate starts the simulation.
    // We pass it the target time. 
    GameStateManager::getInstance()->physScene->Simulate(delTime);
    
    // We want the results immediately, so we call FetchResults with
    // the same time we just asked for, and the second argument as true
    // to block on the results.
    GameStateManager::getInstance()->physScene->FetchResults(delTime, true);

    // Now the actors have been moved, but we need to update the
    // Gamebryo objects. UpdateDestinations does that. Note it is called
    // on the NiPhysXScene that owns the objects being updated.
    GameStateManager::getInstance()->physScene->UpdateDestinations(delTime);
    
    // FInally we update the scene graph.
    GameStateManager::getInstance()->scene->Update(delTime);

	//Check when ball stops rolling, go to next state: reset pins
	NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();

	if( ballActor->getGlobalPosition().z < -13 || ballActor->isSleeping())
	{
		count ++;
		//ballActor->setGlobalPosition( NxVec3(0,-10,0) );
		ballActor->setLinearVelocity( NxVec3(0,0,0) );
		if(count == 10)
			GameStateManager::getInstance()->changeState(ResetPins::getInstance());
	}

}