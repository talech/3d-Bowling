#include "ResetPins.h"
#include "PositionBall.h"

ResetPins ResetPins::mResetPins;

ResetPins::~ResetPins()
{
}

void ResetPins::enter()
{
	NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();
	ballActor->setGlobalPosition(NxVec3(-0.4,0.4,25.2));
		
	
	
}

void ResetPins::exit()
{
	//here we count the knocked pins
	int numPinsDown = 0;
	//Check when ball stops rolling, go to next state: reset pins
	NiPhysXProp* spPinsProp = GameStateManager::getInstance()->physScene->GetPropAt(0);

	for(int i=0; i<10; i++){
		NxActor* pinActor = ((NiPhysXRigidBodyDest*)spPinsProp->GetDestinationAt(i))->GetActor();
		NxVec3 pos = pinActor->getGlobalPosition();
		NxMat33 orient = pinActor->getGlobalOrientation();
		if(pos.y < 0.72){
			pinActor->setGlobalPosition( NxVec3(0,-10,0) );
			numPinsDown+=1;
		}
	}
	
	
	GameStateManager::getInstance()->score->setScoreThrow(numPinsDown);

	int throwNum = GameStateManager::getInstance()->score->getThrow();
	if(throwNum == 2){
		/*NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
		NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();
		ballActor->setGlobalPosition(NxVec3(-0.4,0.4,25.2));
		ballActor->setLinearVelocity( NxVec3(0,0,0) );*/
	}
	else{
		GameStateManager::getInstance()->physScene->RestoreSnapshotState(0);
	}
		
	
}

void ResetPins::pause()
{
}

void ResetPins::resume()
{
}

void ResetPins::processMouse(Mouse *mouse)
{
}

void ResetPins::processKeyboard(Keyboard *keyboard)
{
}

void ResetPins::processGamePad(GamePad *gamepad)
{
}

void ResetPins::update(float delTime)
{
			
	GameStateManager::getInstance()->changeState(PositionBall::getInstance());
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

}