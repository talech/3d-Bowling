#ifndef Bowling_H
#define Bowling_H

#define GAMEBRYO


#include <efdPhysX/PhysXSDKManager.h>

#include <stdio.h>
#include <NiSample.h>
#include <NiFont.h>
#include <Ni2DString.h>
#include <Ni2DStringRenderClick.h>
#include <NiPhysX.h>
#include "SoundSystem.h"
#include "MousePath.h"


#include "GameStateManager.h"

static const int NUM_THROWS = 21;
static const int NUM_TOTALS = 11;

class Bowling : public NiSample
{
public:
    Bowling();
    ~Bowling();
    
    bool Initialize();
    void Terminate();
    void UpdateFrame();
    void ProcessInput();
	//bool CreateUIElements();
	void ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);

	void SetCamera( NiCameraPtr cam );

	SoundSystem soundSystem;

protected:
    // Create all the contents of the scene
    bool CreateScene();
	bool CreateFrame();
    void ResetBall();
    
    // Remap the keys used to control the view.
    void SetTurretControls();

    // Physics related members
    efdPhysX::PhysXSDKManager* m_pkPhysManager;
    NiPhysXScenePtr m_spPhysScene;

    // Keyboard and gamepad handling for the camera.
    NiTurret m_kTurret;
    NiNodePtr m_spTrnNode;
    NiNodePtr m_spRotNode;
	NiPhysXPropPtr spBallProp;
	NiAVObjectPtr spCone;

	//Screen Overlay
	bool CreateScreenElements();
	void DrawScore();
	bool CreateScoreElements();
	unsigned int m_uiFlags;
	unsigned int m_num;
	NiFontPtr m_spFont;
    
    Ni2DStringPtr m_spStrThrows[NUM_THROWS];
	Ni2DStringPtr m_spStrTotals[NUM_TOTALS];
    NiColorA m_kColor;
    NiFixedString m_kUnicodeRenderClickName;

	// MousePath to maintain the direction of the ball
	MousePath m_mousePath;

	// Keep track if the ball is in motion
	bool m_ballInMotion;

	// Physics contact callbacks
    class ContactReporter : public NxUserContactReport    
    {
    public:
        ContactReporter() : m_app(NULL) {}    
        void onContactNotify(NxContactPair& pair, NxU32 events)
        {
            if (m_app) m_app->processContacts(pair, events);
        }
        Bowling* m_app;    
    };
    void processContacts(NxContactPair& pair, NxU32 events);
	ContactReporter m_PhysContactReporter;
};

#endif // #ifdef Bowling_H