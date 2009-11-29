#include "PositionBall.h"
#include "ThrowBall.h"
#include "NewGameMenu.h"

PositionBall PositionBall::mPositionBall;

PositionBall::~PositionBall()
{
}

void PositionBall::enter()
{
	NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();
	ballActor->setGlobalPosition(NxVec3(-0.4,0.4,25.2));

	// Kill the velocity of the ball.
	ballActor->setLinearVelocity(NxVec3(0, 0, 0));
	ballActor->setMass(7.5);
}

void PositionBall::exit()
{
}

void PositionBall::pause()
{
}

void PositionBall::resume()
{
}

void PositionBall::processMouse(Mouse *mouse)
{
	if( mouse )
	{
		if( mouse->ButtonWasPressed(NiInputMouse::Button::NIM_LEFT) )
		{
			if(!GameStateManager::getInstance()->score->isGameOver())
				GameStateManager::getInstance()->changeState( ThrowBall::getInstance() );
		}
	}
}

void PositionBall::processKeyboard(Keyboard *keyboard)
{
	NiPhysXProp* spBallProp = GameStateManager::getInstance()->physScene->GetPropAt(1);
	NxActor* ballActor = ((NiPhysXRigidBodyDest*)spBallProp->GetDestinationAt(0))->GetActor();
	if( keyboard )
	{
		if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_LEFT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT))
		{
			NxVec3 currPos = ballActor->getGlobalPosition();
			if( currPos.x > -2 )
			{
				ballActor->setGlobalPosition( currPos + NxVec3(-0.25, 0, 0) );
			}
		}
		else if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_RIGHT) || keyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT))
		{
			NxVec3 currPos = ballActor->getGlobalPosition();
			if( currPos.x < 2 )
			{
				ballActor->setGlobalPosition( currPos + NxVec3(0.25, 0, 0) );
			}
		}
		else if (keyboard->KeyWasPressed(NiInputKeyboard::KEY_SPACE))
		{
			GameStateManager::getInstance()->changeState( ThrowBall::getInstance() );
		}
	}
}

void PositionBall::processGamePad(GamePad *gamepad)
{
}

void PositionBall::update(float delTime)
{
	if( GameStateManager::getInstance()->score->isGameOver() )
	{
		GameStateManager::getInstance()->lastScore = GameStateManager::getInstance()->score->getTotalScore();
		GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
	}

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