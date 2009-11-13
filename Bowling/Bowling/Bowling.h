#ifndef Bowling_H
#define Bowling_H

#include <efdPhysX/PhysXSDKManager.h>

#include <NiApplication.h>
#include <NiPhysX.h>
#include "SoundSystem.h"


class Bowling : public NiApplication
{
public:
    Bowling();
    ~Bowling();
    
    bool Initialize();
    void Terminate();
    void UpdateFrame();
    void ProcessInput();
	void ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength);

	SoundSystem soundSystem;

protected:
    // Create all the contents of the scene
    bool CreateScene();
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
	NiPhysXPropPtr spCubeProp;
	NiAVObjectPtr spCone;

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